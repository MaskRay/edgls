// Copyright 2026 edgls Authors
// SPDX-License-Identifier: Apache-2.0

#include "edg_args.hh"

#include "log.hh"
#include "project.hh"
#include "utils.hh"

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/Program.h>
#include <llvm/Support/xxhash.h>

#include <llvm/Support/raw_ostream.h>

#include <map>
#include <mutex>
#include <optional>

using namespace llvm;

namespace ccls {
namespace {
// What the front end needs to emulate a compiler: its system include
// directories, version, and default language standard.
struct CompilerInfo {
  bool ok = false;
  bool clang = false;
  int gnu_version = 0;
  int clang_version = 0;
  long cplusplus = 0;
  long stdc_version = 0;
  std::vector<std::string> system_dirs;
  // An EDG base directory whose predefined macro file has the compiler's
  // predefined macros.
  std::string edg_base;
};

std::mutex compiler_mutex;
std::map<std::vector<std::string>, CompilerInfo> compiler_cache;

std::optional<std::string> runAndCapture(StringRef program, ArrayRef<StringRef> argv, bool want_stderr) {
  SmallString<128> in, out;
  if (sys::fs::createTemporaryFile("edgls-compiler", "out", out))
    return std::nullopt;
  std::optional<StringRef> redirects[3] = {StringRef(""), StringRef(""), StringRef("")};
  redirects[want_stderr ? 2 : 1] = StringRef(out);
  std::string err;
  int rc = sys::ExecuteAndWait(program, argv, std::nullopt, redirects, 30, 0, &err);
  std::optional<std::string> ret;
  if (rc == 0)
    if (auto buf = MemoryBuffer::getFile(out))
      ret = (*buf)->getBuffer().str();
  sys::fs::remove(out);
  return ret;
}

// Macros that the front end defines itself (see EDG's
// util/make_predef_macro_table).
bool isFrontEndMacro(StringRef name, bool clang) {
  static const char *const names[] = {
      "__cplusplus",           "__STDC_HOSTED__",         "__STDC_VERSION__",
      "__STDC__",              "__STDC_IEC_559__",        "__STDC_IEC_559_COMPLEX__",
      "__STDC_MB_MIGHT_NEQ_WC__", "__STDC_NO_ATOMICS__",  "__STDC_NO_THREADS__",
      "__STDC_NO_VLA__",       "__STDC_UTF_16__",         "__STDC_UTF_32__",
      "__STDCPP_THREADS__",    "__STDCPP_DEFAULT_NEW_ALIGNMENT__", "__GXX_RTTI",
      "__GNUC_GNU_INLINE__",   "__GNUC_STDC_INLINE__",    "__CHAR16_TYPE__",
      "__CHAR32_TYPE__",       "__EXCEPTIONS",            "__GLIBCXX_BITSIZE_INT_N_0",
      "__GLIBCXX_TYPE_INT_N_0", "__CHAR8_TYPE__",         "__GCC_ATOMIC_CHAR8_T_LOCK_FREE"};
  for (const char *n : names)
    if (name == n)
      return true;
  if (name.starts_with("__cpp_") || name.contains('('))
    return true;
  if (clang)
    return name == "__clang__" || name.starts_with("__clang_major__") || name.starts_with("__clang_minor__") ||
           name.starts_with("__clang_patchlevel__") || name == "__clang_version__";
  return name == "__GNUC__" || name == "__GNUC_MINOR__" || name == "__GNUC_PATCHLEVEL__" || name == "__GNUG__" ||
         name == "__VERSION__";
}

// Write the compiler's predefined macros to <dir>/lib/predefined_macros.txt
// and return <dir>.
std::string writeEdgBase(StringRef macros, bool clang) {
  std::string content;
  SmallVector<StringRef, 0> lines;
  macros.split(lines, '\n');
  for (StringRef line : lines) {
    if (!line.consume_front("#define "))
      continue;
    auto [name, value] = line.split(' ');
    if (name.empty() || isFrontEndMacro(name, clang))
      continue;
    content += "gnu_or_clang no ";
    content += name;
    content += ' ';
    content += value;
    content += '\n';
  }
  SmallString<128> dir;
  if (!sys::path::cache_directory(dir))
    return "";
  sys::path::append(dir, "edgls", std::to_string(llvm::xxh3_64bits(content)));
  SmallString<128> lib(dir);
  sys::path::append(lib, "lib");
  if (sys::fs::create_directories(lib, true, sys::fs::perms::owner_all))
    return "";
  sys::path::append(lib, "predefined_macros.txt");
  if (!sys::fs::exists(lib)) {
    // Write atomically as other processes may read the file.
    int fd;
    SmallString<128> tmp;
    if (sys::fs::createUniqueFile(std::string(lib) + ".%%%%%%", fd, tmp))
      return "";
    {
      raw_fd_ostream os(fd, /*shouldClose=*/true);
      os << content;
      if (os.has_error()) {
        os.clear_error();
        sys::fs::remove(tmp);
        return "";
      }
    }
    if (sys::fs::rename(tmp, lib)) {
      sys::fs::remove(tmp);
      return "";
    }
  }
  return std::string(dir);
}

CompilerInfo queryCompiler(const std::vector<std::string> &key) {
  // key: compiler, language, flags affecting the search path
  CompilerInfo info;
  auto program = sys::findProgramByName(key[0]);
  if (!program) {
    LOG_S(WARNING) << "cannot find compiler " << key[0];
    return info;
  }
  std::vector<StringRef> argv{*program};
  for (size_t i = 2; i < key.size(); i++)
    argv.push_back(key[i]);
  argv.insert(argv.end(), {"-x", key[1], "-E", "-v", "-dM", "-"});
  // stdout has the macros and stderr has the search list.
  std::optional<std::string> macros = runAndCapture(*program, argv, false);
  std::optional<std::string> verbose = runAndCapture(*program, argv, true);
  if (!macros || !verbose) {
    LOG_S(WARNING) << "failed to query compiler " << key[0];
    return info;
  }
  int gnu[3] = {}, clang[3] = {};
  SmallVector<StringRef, 0> lines;
  StringRef(*macros).split(lines, '\n');
  for (StringRef line : lines) {
    if (!line.consume_front("#define "))
      continue;
    auto [name, value] = line.split(' ');
    long v = 0;
    StringRef vv = value.rtrim("L");
    if (vv.getAsInteger(10, v))
      continue;
    if (name == "__GNUC__")
      gnu[0] = (int)v;
    else if (name == "__GNUC_MINOR__")
      gnu[1] = (int)v;
    else if (name == "__GNUC_PATCHLEVEL__")
      gnu[2] = (int)v;
    else if (name == "__clang_major__")
      clang[0] = (int)v, info.clang = true;
    else if (name == "__clang_minor__")
      clang[1] = (int)v;
    else if (name == "__clang_patchlevel__")
      clang[2] = (int)v;
    else if (name == "__cplusplus")
      info.cplusplus = v;
    else if (name == "__STDC_VERSION__")
      info.stdc_version = v;
  }
  info.gnu_version = gnu[0] * 10000 + gnu[1] * 100 + gnu[2];
  info.clang_version = clang[0] * 10000 + clang[1] * 100 + clang[2];
  info.edg_base = writeEdgBase(*macros, info.clang);
  if (info.edg_base.empty())
    LOG_S(WARNING) << "cannot write the predefined macros of " << key[0] << "; using the default EDG base directory";

  lines.clear();
  StringRef(*verbose).split(lines, '\n');
  bool in_list = false;
  for (StringRef line : lines) {
    if (line.starts_with("#include <...> search starts here:")) {
      in_list = true;
      continue;
    }
    if (line.starts_with("End of search list."))
      break;
    if (in_list) {
      line = line.trim();
      line.consume_back(" (framework directory)");
      SmallString<256> dir(line);
      sys::path::remove_dots(dir, true);
      info.system_dirs.push_back(std::string(dir));
    }
  }
  info.ok = true;
  return info;
}

const CompilerInfo &getCompilerInfo(const std::vector<std::string> &key) {
  std::lock_guard lock(compiler_mutex);
  auto it = compiler_cache.find(key);
  if (it == compiler_cache.end()) {
    it = compiler_cache.emplace(key, queryCompiler(key)).first;
    const CompilerInfo &info = it->second;
    if (info.ok)
      LOG_S(INFO) << "compiler " << key[0] << " (" << key[1] << "): " << (info.clang ? "clang " : "gnu ")
                  << (info.clang ? info.clang_version : info.gnu_version) << ", " << info.system_dirs.size()
                  << " system include directories";
  }
  return it->second;
}

std::string absolute(const std::string &dir, StringRef path) {
  SmallString<256> ret;
  if (!sys::path::is_absolute(path))
    ret = dir;
  sys::path::append(ret, path);
  sys::path::remove_dots(ret, true);
  return std::string(ret);
}

const char *cxxStd(StringRef s) {
  s.consume_front("gnu++");
  s.consume_front("c++");
  if (s == "98" || s == "03")
    return "--c++03";
  if (s == "11" || s == "0x")
    return "--c++11";
  if (s == "14" || s == "1y")
    return "--c++14";
  if (s == "17" || s == "1z")
    return "--c++17";
  if (s == "20" || s == "2a")
    return "--c++20";
  if (s == "23" || s == "2b")
    return "--c++23";
  if (s == "26" || s == "2c")
    return "--c++26";
  return nullptr;
}

const char *cStd(StringRef s) {
  if (s == "c89" || s == "c90" || s == "gnu89" || s == "gnu90" || s == "iso9899:1990")
    return "--c89";
  if (s == "c99" || s == "c9x" || s == "gnu99" || s == "gnu9x" || s == "iso9899:1999")
    return "--c99";
  if (s == "c11" || s == "c1x" || s == "gnu11" || s == "gnu1x" || s == "iso9899:2011")
    return "--c11";
  if (s == "c17" || s == "c18" || s == "gnu17" || s == "gnu18" || s == "iso9899:2017" || s == "iso9899:2018")
    return "--c17";
  if (s == "c2x" || s == "c23" || s == "gnu2x" || s == "gnu23" || s == "c2y" || s == "gnu2y")
    return "--c23";
  return nullptr;
}

const char *cxxStdFromMacro(long v) {
  if (v >= 202400)
    return "--c++26";
  if (v >= 202302)
    return "--c++23";
  if (v >= 202002)
    return "--c++20";
  if (v >= 201703)
    return "--c++17";
  if (v >= 201402)
    return "--c++14";
  if (v >= 201103)
    return "--c++11";
  return "--c++03";
}

const char *cStdFromMacro(long v) {
  if (v >= 202311)
    return "--c23";
  if (v >= 201710)
    return "--c17";
  if (v >= 201112)
    return "--c11";
  if (v >= 199901)
    return "--c99";
  return "--c89";
}

// Options whose argument is the next command line argument and which do not
// affect the front end.
bool skipsNextArg(StringRef a) {
  static const char *const opts[] = {"-o",          "-MF",         "-MT",          "-MQ",         "-MJ",
                                     "-Xclang",     "-Xlinker",    "-Xassembler",  "-Xcuda-ptxas", "-Xarch_host",
                                     "-arch",       "-L",          "-T",           "-z",          "-u",
                                     "-e",          "--param",     "-aux-info",    "-dumpbase",   "-dumpdir",
                                     "-install_name", "-framework", "-F",          "-iprefix",    "-iwithprefix",
                                     "-iwithprefixbefore", "-ivfsoverlay", "-iframework", "-gcc-toolchain"};
  for (const char *o : opts)
    if (a == o)
      return true;
  return false;
}
} // namespace

bool buildEdgInvocation(const std::vector<const char *> &args, const std::string &main, EdgInvocation &inv) {
  if (args.empty())
    return false;
  size_t i = 0;
  // Skip compiler wrappers.
  while (i + 1 < args.size()) {
    StringRef base = sys::path::filename(args[i]);
    if (base == "ccache" || base == "sccache" || base == "distcc" || base == "icecc")
      i++;
    else
      break;
  }
  std::string compiler = args[i++];
  StringRef compiler_base = sys::path::filename(compiler);

  std::string dir;
  std::string lang_opt;
  std::vector<std::string> search_flags;
  std::vector<std::string> out, sys_after;
  const char *std_opt = nullptr;
  bool nostdinc = false, nostdincxx = false, no_warnings = false;

  auto next = [&](size_t &j) -> std::optional<std::string> {
    if (j + 1 < args.size())
      return std::string(args[++j]);
    return std::nullopt;
  };
  // First pass for -working-directory, which applies to all relative paths.
  for (size_t j = i; j < args.size(); j++) {
    StringRef a = args[j];
    if (a.consume_front("-working-directory=")) {
      dir = a.str();
    } else if (a == "-working-directory") {
      if (auto v = next(j))
        dir = *v;
    }
  }
  if (dir.empty())
    dir = sys::path::parent_path(main).str();

  for (size_t j = i; j < args.size(); j++) {
    StringRef a = args[j];
    if (a.empty() || a[0] != '-')
      continue; // inputs
    auto joinedOrSep = [&](StringRef opt, std::string &val) {
      if (!a.starts_with(opt))
        return false;
      if (a.size() > opt.size()) {
        val = a.substr(opt.size()).str();
        if (val[0] == '=' && (opt == "--include-directory" || opt == "--define-macro"))
          val.erase(0, 1);
        return true;
      }
      if (auto v = next(j)) {
        val = *v;
        return true;
      }
      return false;
    };
    std::string v;
    if (a.starts_with("-working-directory")) {
      if (a == "-working-directory")
        j++;
    } else if (a == "-isystem" || a == "-idirafter" || a == "-isystem-after") {
      if (auto n = next(j)) {
        auto &to = a == "-isystem" ? out : sys_after;
        to.insert(to.end(), {"--sys_include", absolute(dir, *n)});
      }
    } else if (a.starts_with("-isystem")) {
      out.insert(out.end(), {"--sys_include", absolute(dir, a.substr(8))});
    } else if (a.starts_with("-idirafter")) {
      sys_after.insert(sys_after.end(), {"--sys_include", absolute(dir, a.substr(10))});
    } else if (a == "-iquote" || (a.starts_with("-iquote"))) {
      if (a == "-iquote") {
        if (auto n = next(j))
          out.insert(out.end(), {"-I", absolute(dir, *n)});
      } else {
        out.insert(out.end(), {"-I", absolute(dir, a.substr(7))});
      }
    } else if (a == "-I-") {
    } else if (joinedOrSep("--include-directory", v) || joinedOrSep("-I", v)) {
      out.insert(out.end(), {"-I", absolute(dir, v)});
    } else if (joinedOrSep("--define-macro", v) || joinedOrSep("-D", v)) {
      out.push_back("-D" + v);
    } else if (joinedOrSep("--undefine-macro", v) || joinedOrSep("-U", v)) {
      out.push_back("-U" + v);
    } else if (a == "-include" || a.starts_with("--include=")) {
      if (a == "-include") {
        if (auto n = next(j))
          out.insert(out.end(), {"--preinclude", absolute(dir, *n)});
      } else {
        out.insert(out.end(), {"--preinclude", absolute(dir, a.substr(10))});
      }
    } else if (a == "-imacros") {
      if (auto n = next(j))
        out.insert(out.end(), {"--preinclude_macros", absolute(dir, *n)});
    } else if (a == "-x") {
      if (auto n = next(j))
        lang_opt = *n;
    } else if (a.starts_with("-x")) {
      lang_opt = a.substr(2).str();
    } else if (a.consume_front("-std=") || a.consume_front("--std=")) {
      std_opt = cxxStd(a);
      if (!std_opt)
        std_opt = cStd(a);
    } else if (a == "-ansi") {
      std_opt = "ansi";
    } else if (a == "-nostdinc") {
      nostdinc = true;
      search_flags.push_back(a.str());
    } else if (a == "-nostdinc++" || a == "-nostdlibinc") {
      nostdincxx = true;
      search_flags.push_back(a.str());
    } else if (a == "-w") {
      no_warnings = true;
    } else if (a == "-fno-exceptions") {
      out.push_back("--no_exceptions");
    } else if (a == "-fexceptions") {
      out.push_back("--exceptions");
    } else if (a == "-fno-rtti") {
      out.push_back("--no_rtti");
    } else if (a == "-frtti") {
      out.push_back("--rtti");
    } else if (a == "-funsigned-char") {
      out.push_back("--unsigned_chars");
    } else if (a == "-fsigned-char") {
      out.push_back("--signed_chars");
    } else if (a == "-pthread") {
      out.push_back("-D_REENTRANT");
    } else if (a == "-fopenmp") {
      out.push_back("-D_OPENMP=201511");
    } else if (a == "-target" || a == "--sysroot" || a == "-isysroot" || a == "--gcc-toolchain" ||
               a == "--gcc-install-dir") {
      search_flags.push_back(a.str());
      if (auto n = next(j))
        search_flags.push_back(*n);
    } else if (a.starts_with("--target=") || a.starts_with("--sysroot=") || a.starts_with("-isysroot") ||
               a.starts_with("-stdlib=") || a.starts_with("--gcc-toolchain=") ||
               a.starts_with("--gcc-install-dir=") || a == "-m32" || a == "-m64" || a == "-mx32" ||
               a.starts_with("-march=") || a.starts_with("--config")) {
      search_flags.push_back(a.str());
    } else if (a == "-Xpreprocessor") {
      j++;
    } else if (skipsNextArg(a)) {
      j++;
    }
  }

  // Determine the language.
  bool cxx;
  if (lang_opt.size()) {
    StringRef l = lang_opt;
    if (l == "c" || l == "c-header" || l == "cpp-output")
      cxx = false;
    else if (l == "c++" || l == "c++-header" || l == "c++-cpp-output" || l == "cuda" || l == "objective-c++" ||
             l == "objective-c++-header")
      cxx = true;
    else
      return false;
  } else {
    auto [lang, header] = lookupExtension(main);
    if (lang == LanguageId::C)
      cxx = false;
    else if (lang == LanguageId::Cpp || lang == LanguageId::Cuda || lang == LanguageId::ObjCpp)
      cxx = true;
    else
      return false;
  }
  // The driver name (g++, g++-13, clang++) or a C++ standard imply the
  // language of headers.
  if (!cxx && lang_opt.empty() && lookupExtension(main).second &&
      (compiler_base.contains("++") || (std_opt && StringRef(std_opt).starts_with("--c++"))))
    cxx = true;
  inv.lang = cxx ? LanguageId::Cpp : LanguageId::C;

  std::vector<std::string> key{compiler, cxx ? "c++" : "c"};
  key.insert(key.end(), search_flags.begin(), search_flags.end());
  const CompilerInfo *info = &getCompilerInfo(key);
  if (!info->ok) {
    key[0] = cxx ? "c++" : "cc";
    info = &getCompilerInfo(key);
  }

  std::vector<std::string> &res = inv.args;
  res.clear();
  if (info->edg_base.size()) {
    res.push_back("--edg_base_dir");
    res.push_back(info->edg_base);
  }
  if (info->clang) {
    // The front end picks a GNU version matching the clang version.
    res.push_back("--clang");
    res.push_back("--clang_version=" + std::to_string(info->clang_version));
  } else {
    res.push_back(cxx ? "--g++" : "--gcc");
    res.push_back("--gnu_version=" + std::to_string(info->gnu_version ? info->gnu_version : 140000));
  }
  if (!cxx) {
    res.push_back("--c");
    if (std_opt && StringRef(std_opt) == "ansi")
      std_opt = "--c89";
    if (!std_opt || StringRef(std_opt).starts_with("--c++"))
      std_opt = info->stdc_version ? cStdFromMacro(info->stdc_version) : "--c17";
  } else {
    if (std_opt && StringRef(std_opt) == "ansi")
      std_opt = "--c++03";
    if (!std_opt || !StringRef(std_opt).starts_with("--c++"))
      std_opt = info->cplusplus ? cxxStdFromMacro(info->cplusplus) : "--c++17";
  }
  res.push_back(std_opt);
  if (no_warnings)
    res.push_back("--no_warnings");
  res.insert(res.end(), out.begin(), out.end());
  if (!nostdinc)
    for (const std::string &d : info->system_dirs) {
      if (nostdincxx && StringRef(d).contains("/c++/"))
        continue;
      res.push_back("--sys_include");
      res.push_back(d);
    }
  res.insert(res.end(), sys_after.begin(), sys_after.end());
  res.push_back(main);
  return true;
}
} // namespace ccls
