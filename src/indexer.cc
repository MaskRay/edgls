// Copyright 2017-2018 ccls Authors
// SPDX-License-Identifier: Apache-2.0

#include "indexer.hh"

#include "edg/edg.hh"
#include "edg_args.hh"
#include "log.hh"
#include "pipeline.hh"
#include "platform.hh"
#include "sema_manager.hh"

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/CrashRecoveryContext.h>
#include <llvm/Support/Path.h>

#include <algorithm>
#include <deque>
#include <inttypes.h>
#include <unordered_set>

using namespace llvm;

namespace ccls {
namespace {
GroupMatch *multiVersionMatcher;

bool isIdentChar(unsigned char c) { return isalnum(c) || c == '_' || c == '$' || c >= 0x80; }

// Source text of a file with line offsets, for converting front end positions
// (1-based line and column) to ccls positions (0-based). Columns count code
// points.
struct Text {
  std::string content;
  std::vector<uint32_t> lines;

  void init(std::string c) {
    content = std::move(c);
    lines.assign(1, 0);
    for (size_t i = 0; i < content.size(); i++)
      if (content[i] == '\n')
        lines.push_back(uint32_t(i + 1));
  }
  // Byte offset of a 1-based line and column (counted in code points), or
  // npos.
  size_t offset(int line, int column) const {
    if (line < 1 || line > (int)lines.size())
      return std::string::npos;
    size_t off = lines[line - 1];
    for (int col = 1; col < column && off < content.size() && content[off] != '\n'; col++)
      do
        off++;
      while (off < content.size() && ((uint8_t)content[off] & 0xc0) == 0x80);
    return off;
  }
  Pos pos(size_t off) const {
    off = std::min(off, content.size());
    int line = int(std::upper_bound(lines.begin(), lines.end(), uint32_t(off)) - lines.begin()) - 1;
    int col = 0;
    for (size_t i = lines[line]; i < off; i++)
      if (((uint8_t)content[i] & 0xc0) != 0x80)
        col++;
    return {(uint16_t)std::min(line, (int)UINT16_MAX), (int16_t)std::min(col, (int)INT16_MAX)};
  }
};

const char *const kOperators[] = {
    "<<=", ">>=", "<=>", "->*", "...", "::", "->", "++", "--", "<<", ">>", "<=", ">=", "==", "!=", "&&", "||",
    "+=",  "-=",  "*=",  "/=",  "%=",  "&=", "|=", "^=", ".*", "()", "[]", "+",  "-",  "*",  "/",  "%",  "^",
    "&",   "|",   "~",   "!",   "=",   "<",  ">",  ",",  "(",  ")",  "[",  "]",  "{",  "}",  ";",  ":",  "."};

// The end offset of the token starting at |off|. Operator function names
// ("operator+=", "operator new[]", "~Foo") are treated as one token.
size_t tokenEnd(const std::string &s, size_t off) {
  size_t n = s.size(), i = off;
  if (i >= n)
    return i;
  if (s[i] == '~') {
    size_t j = i + 1;
    while (j < n && (s[j] == ' ' || s[j] == '\t'))
      j++;
    if (j < n && isIdentChar(s[j]) && !isdigit((unsigned char)s[j])) {
      while (j < n && isIdentChar(s[j]))
        j++;
      return j;
    }
  }
  if (isIdentChar(s[i])) {
    while (i < n && isIdentChar(s[i]))
      i++;
    if (s.compare(off, i - off, "operator") != 0)
      return i;
    size_t j = i;
    while (j < n && (s[j] == ' ' || s[j] == '\t'))
      j++;
    if (j < n && isIdentChar(s[j])) {
      // operator new, operator delete, or a conversion function.
      size_t k = j;
      while (k < n && isIdentChar(s[k]))
        k++;
      StringRef word(s.data() + j, k - j);
      if (word == "new" || word == "delete") {
        size_t l = k;
        while (l < n && (s[l] == ' ' || s[l] == '\t'))
          l++;
        if (l + 1 < n && s[l] == '[' && s[l + 1] == ']')
          return l + 2;
      }
      return k;
    }
    if (j + 1 < n && s[j] == '"' && s[j + 1] == '"') {
      size_t k = j + 2;
      while (k < n && (s[k] == ' ' || s[k] == '\t'))
        k++;
      while (k < n && isIdentChar(s[k]))
        k++;
      return k;
    }
    for (const char *op : kOperators)
      if (s.compare(j, strlen(op), op) == 0)
        return j + strlen(op);
    return i;
  }
  for (const char *op : kOperators)
    if (s.compare(i, strlen(op), op) == 0)
      return i + strlen(op);
  return i + 1;
}

// Skip a comment, string literal, or character literal starting at |i|.
// Returns |i| if there is none.
size_t skipNonCode(const std::string &s, size_t i) {
  size_t n = s.size();
  if (s[i] == '/' && i + 1 < n && s[i + 1] == '/') {
    while (i < n && s[i] != '\n')
      i++;
    return i;
  }
  if (s[i] == '/' && i + 1 < n && s[i + 1] == '*') {
    size_t e = s.find("*/", i + 2);
    return e == std::string::npos ? n : e + 2;
  }
  // A digit separator, e.g. 1'000: the quote is inside a pp-number, which
  // starts with a digit (unlike the prefix of L'a' or u8'a').
  if (s[i] == '\'' && i + 1 < n && isalnum((unsigned char)s[i + 1])) {
    size_t j = i;
    while (j > 0 && (isalnum((unsigned char)s[j - 1]) || s[j - 1] == '_' || s[j - 1] == '\'' || s[j - 1] == '.'))
      j--;
    if (j < i && isdigit((unsigned char)s[j]))
      return i;
  }
  if (s[i] == '"' || s[i] == '\'') {
    // Raw string literal.
    if (s[i] == '"' && i && s[i - 1] == 'R') {
      size_t paren = s.find('(', i);
      if (paren != std::string::npos) {
        std::string delim = ")" + s.substr(i + 1, paren - i - 1) + "\"";
        size_t e = s.find(delim, paren);
        return e == std::string::npos ? n : e + delim.size();
      }
    }
    char q = s[i++];
    while (i < n && s[i] != q && s[i] != '\n') {
      if (s[i] == '\\')
        i++;
      i++;
    }
    return std::min(i + 1, n);
  }
  return i;
}

// Find the end of a declaration whose name ends at |off|: the closing brace
// of a body, or the last token before ';', ',' or an unbalanced ')'.
// Returns the offset just past the last character.
size_t declEnd(const std::string &s, size_t off, bool is_def, bool is_param) {
  size_t n = s.size(), last = off;
  int depth = 0;
  // Within a mem-initializer list, '{' after an identifier starts an
  // initializer rather than the function body.
  bool mem_init = false;
  char prev = 0;
  for (size_t i = off; i < n;) {
    size_t j = skipNonCode(s, i);
    if (j != i) {
      if (s[i] == '"' || s[i] == '\'')
        last = j, prev = 'a';
      i = j;
      continue;
    }
    char c = s[i];
    if (isspace((unsigned char)c)) {
      i++;
      continue;
    }
    if (c == '#' && (i == 0 || s[i - 1] == '\n'))
      break;
    if (depth == 0) {
      if (c == ';' || (c == ',' && (is_param || !mem_init)) || c == ')' || c == '}' || c == ']')
        break;
      if (c == ':' && i + 1 < n && s[i + 1] == ':') {
        i += 2, last = i, prev = ':';
        continue;
      }
      if (c == ':' && !is_param)
        mem_init = true;
      if (c == '{' && is_def && !(mem_init && (isIdentChar(prev) || prev == '>'))) {
        // The body.
        int d = 0;
        for (; i < n;) {
          size_t k = skipNonCode(s, i);
          if (k != i) {
            i = k;
            continue;
          }
          if (s[i] == '{')
            d++;
          else if (s[i] == '}' && --d == 0)
            return i + 1;
          i++;
        }
        return n;
      }
    }
    if (c == '(' || c == '[' || c == '{')
      depth++;
    else if (c == ')' || c == ']' || c == '}')
      depth--;
    i++;
    last = i;
    prev = c;
  }
  return last;
}

// Find the start of the declaration containing the name at |off|.
size_t declBegin(const std::string &s, size_t off, bool is_param) {
  // Scan backward to the previous ';', '{', '}' or an access specifier,
  // ignoring comments and preprocessor lines.
  size_t i = off, begin = off;
  int depth = 0;
  while (i > 0) {
    char c = s[i - 1];
    // Ignore the rest of a line comment.
    size_t ls = s.rfind('\n', i - 1);
    ls = ls == std::string::npos ? 0 : ls + 1;
    if (size_t cmt = ls + 1 < i ? StringRef(s.data() + ls, i - 1 - ls).find("//") : StringRef::npos;
        cmt != StringRef::npos) {
      i = ls + cmt;
      continue;
    }
    if (c == '/' && i >= 2 && s[i - 2] == '*') {
      size_t st = s.rfind("/*", i - 2);
      if (st == std::string::npos)
        break;
      i = st;
      continue;
    }
    if (s[ls] == '#' && ls < i - 1) {
      // Preprocessor line.
      break;
    }
    if (c == ')' || c == ']' || c == '>')
      depth++;
    else if (c == '(' || c == '[' || c == '<') {
      if (depth == 0)
        break;
      depth--;
    } else if (depth == 0 && (c == ';' || c == '{' || c == '}' || (is_param && c == ',')))
      break;
    else if (depth == 0 && c == ':' && !(i >= 2 && s[i - 2] == ':') && !(i < s.size() && s[i] == ':')) {
      // public: / case x: / label:
      break;
    }
    i--;
    if (!isspace((unsigned char)c))
      begin = i;
  }
  return begin;
}

// The comment block immediately preceding offset |begin| (on the previous
// lines) or a trailing ///< comment.
std::string getComment(const std::string &s, size_t begin, size_t end) {
  // Trailing comment on the same line.
  size_t eol = s.find('\n', end);
  if (eol == std::string::npos)
    eol = s.size();
  StringRef rest(s.data() + end, eol - end);
  size_t tc = rest.find("///<");
  if (tc == StringRef::npos)
    tc = rest.find("//!<");
  if (tc != StringRef::npos)
    return rest.substr(tc + 4).trim().str();

  size_t ls = s.rfind('\n', begin ? begin - 1 : 0);
  if (ls == std::string::npos || begin == 0)
    return "";
  std::vector<std::string> lines;
  size_t line_end = ls;
  bool block = false;
  while (line_end != std::string::npos && line_end > 0) {
    size_t line_start = s.rfind('\n', line_end - 1);
    line_start = line_start == std::string::npos ? 0 : line_start + 1;
    StringRef line = StringRef(s.data() + line_start, line_end - line_start).trim();
    if (block) {
      lines.push_back(line.str());
      if (line.starts_with("/*"))
        break;
    } else if (line.starts_with("//")) {
      lines.push_back(line.str());
    } else if (line.ends_with("*/")) {
      lines.push_back(line.str());
      if (line.starts_with("/*"))
        break;
      block = true;
    } else {
      break;
    }
    if (line_start == 0)
      break;
    line_end = line_start - 1;
  }
  std::reverse(lines.begin(), lines.end());
  std::string ret;
  for (std::string &line : lines) {
    StringRef l = line;
    if (l.starts_with("//")) {
      l = l.drop_while([](char c) { return c == '/' || c == '!' || c == '<'; });
    } else {
      if (l.starts_with("/*"))
        l = l.drop_front(2).drop_while([](char c) { return c == '*' || c == '!' || c == '<'; });
      if (l.ends_with("*/"))
        l = l.drop_back(2).rtrim('*');
      if (l.starts_with("*"))
        l = l.drop_front(1);
    }
    if (l.starts_with(" "))
      l = l.drop_front(1);
    if (ret.size())
      ret += '\n';
    ret += l.rtrim().str();
  }
  while (ret.size() && isspace((unsigned char)ret.back()))
    ret.pop_back();
  while (ret.size() && isspace((unsigned char)ret[0]))
    ret.erase(0, 1);
  return ret;
}

struct File {
  std::string path;
  int64_t mtime = 0;
  Text text;
  std::unique_ptr<IndexFile> db;
};

struct EntityInfo {
  Usr usr = 0;
  bool hashed = false;
};

struct IndexParam {
  VFS &vfs;
  bool no_linkage;
  edg::Unit &unit;
  const std::vector<std::pair<std::string, std::string>> &remapped;
  std::deque<File> files;
  // The front end has a file id for each inclusion of a file.
  std::vector<File *> fid2file;
  std::unordered_map<std::string, File *> path2file;
  std::vector<EntityInfo> entities;

  IndexParam(VFS &vfs, bool no_linkage, edg::Unit &unit,
             const std::vector<std::pair<std::string, std::string>> &remapped)
      : vfs(vfs), no_linkage(no_linkage), unit(unit), remapped(remapped) {}

  File &file(int fid) {
    if ((size_t)fid >= fid2file.size())
      fid2file.resize(fid + 1);
    if (File *f = fid2file[fid])
      return *f;
    std::string path = unit.files[fid].path;
    std::string real = realPath(path);
    if (real.size())
      path = real;
    normalizeFolder(path);
    auto [it, inserted] = path2file.try_emplace(path);
    if (inserted) {
      File &f = files.emplace_back();
      it->second = &f;
      f.path = path;
      if (auto t = lastWriteTime(path))
        f.mtime = *t;
      const std::string *buffer = nullptr;
      for (auto &[name, content] : remapped)
        if (name == path || name == unit.files[fid].path || realPath(name) == path)
          buffer = &content;
      if (buffer)
        f.text.init(*buffer);
      else if (std::optional<std::string> content = readContent(path))
        f.text.init(std::move(*content));
      else
        f.text.init("");
      if (vfs.stamp(path, f.mtime, no_linkage ? 3 : 1))
        f.db = std::make_unique<IndexFile>(path, f.text.content, no_linkage);
    }
    return *(fid2file[fid] = it->second);
  }

  IndexFile *consumeFile(int fid) { return fid >= 0 ? file(fid).db.get() : nullptr; }

  Usr usr(int e) {
    if ((size_t)e >= entities.size())
      entities.resize(unit.entities.size());
    EntityInfo &info = entities[e];
    if (!info.hashed) {
      info.hashed = true;
      info.usr = hashUsr(unit.entities[e].usr);
    }
    return info.usr;
  }
};

Kind toKind(edg::EntityKind k) { return Kind(uint8_t(k)); }

template <typename Def> void setName(Def &def, const edg::Entity &e) {
  def.detailed_name = intern(e.detailed_name);
  def.qual_name_offset = e.qual_name_offset;
  def.short_name_offset = e.short_name_offset;
  def.short_name_size = e.short_name_size;
}

class Indexer {
  IndexParam &param;
  edg::Unit &unit;

public:
  Indexer(IndexParam &param) : param(param), unit(param.unit) {}

  Range tokenRange(File &f, const edg::Loc &loc, size_t *off_begin = nullptr, size_t *off_end = nullptr) {
    size_t b = f.text.offset(loc.line, loc.column);
    if (b == std::string::npos)
      return {};
    size_t e = tokenEnd(f.text.content, b);
    if (off_begin)
      *off_begin = b;
    if (off_end)
      *off_end = e;
    return {f.text.pos(b), f.text.pos(e)};
  }

  void addOccurrence(const edg::Occurrence &o) {
    IndexFile *db = param.consumeFile(o.loc.file);
    if (!db)
      return;
    File &f = param.file(o.loc.file);
    const edg::Entity &e = unit.entities[o.entity];
    if (!param.no_linkage && e.is_local)
      return;
    Usr usr = param.usr(o.entity);
    Kind kind = toKind(e.kind);
    bool implicit = o.role & edg::RoleImplicit;
    if (kind == Kind::Func && implicit && (e.short_name == "operator new" || e.short_name == "operator delete" ||
                                           e.short_name == "operator new[]" || e.short_name == "operator delete[]"))
      return;
    size_t ob, oe;
    Range range = tokenRange(f, o.loc, &ob, &oe);
    if (!range.valid())
      return;
    const std::string &text = f.text.content;
    // An implicit constructor call is reported at the initializer, e.g. the
    // "(" in "new Foo()". Attribute it to the type name.
    if (kind == Kind::Func && implicit && e.symbol_kind == (uint8_t)SymbolKind::Constructor &&
        (text[ob] == '(' || text[ob] == '{')) {
      size_t i = ob;
      while (i > 0 && isspace((unsigned char)text[i - 1]))
        i--;
      size_t j = i;
      while (j > 0 && isIdentChar(text[j - 1]))
        j--;
      if (j < i) {
        ob = j, oe = i;
        range = {f.text.pos(ob), f.text.pos(oe)};
      }
    }
    Role role = Role(o.role);
    bool is_def = o.role & edg::RoleDefinition, is_decl = o.role & edg::RoleDeclaration;
    // A namespace has many definitions; index them as declarations.
    if (is_def && e.symbol_kind == (uint8_t)SymbolKind::Namespace) {
      is_def = false, is_decl = true;
      role = Role((o.role & ~edg::RoleDefinition) | edg::RoleDeclaration);
    }
    if (kind == Kind::Func && !is_def && !is_decl && !(o.role & edg::RoleAddress))
      role = role | Role::Call;

    auto extentOf = [&]() -> Range {
      if (o.extent_begin.valid()) {
        size_t b = f.text.offset(o.extent_begin.line, o.extent_begin.column),
               en = f.text.offset(o.extent_end.line, o.extent_end.column);
        if (b != std::string::npos && en != std::string::npos && b <= en) {
          en = tokenEnd(text, en);
          // The front end may not know the body of a definition (e.g. a
          // template's).
          if (is_def && (kind == Kind::Func || kind == Kind::Type) && en >= oe) {
            size_t i = en;
            for (;;) {
              while (i < text.size() && isspace((unsigned char)text[i]))
                i++;
              size_t j = i < text.size() ? skipNonCode(text, i) : i;
              if (j == i || text[i] == '"' || text[i] == '\'')
                break;
              i = j;
            }
            if (i < text.size() && (text[i] == '{' || (text[i] == ':' && text.compare(i, 2, "::"))))
              en = std::max(en, declEnd(text, i, true, false));
            else if (i < text.size() && text[i] == '=' &&
                     (kind == Kind::Func || e.symbol_kind == (uint8_t)SymbolKind::TypeAlias))
              en = std::max(en, declEnd(text, i, false, false));
          }
          return {f.text.pos(b), f.text.pos(en)};
        }
      }
      if (e.symbol_kind == (uint8_t)SymbolKind::Macro) {
        size_t eol = f.text.content.find('\n', oe);
        while (eol != std::string::npos && eol > 0 && f.text.content[eol - 1] == '\\')
          eol = f.text.content.find('\n', eol + 1);
        size_t start = f.text.content.rfind('#', ob);
        if (start == std::string::npos)
          start = ob;
        return {f.text.pos(start), f.text.pos(eol == std::string::npos ? f.text.content.size() : eol)};
      }
      bool is_param = e.symbol_kind == (uint8_t)SymbolKind::Parameter;
      size_t b = declBegin(f.text.content, ob, is_param);
      size_t en = declEnd(f.text.content, oe, is_def && kind != Kind::Var, is_param);
      if (kind == Kind::Var && is_def && !is_param) {
        // Include a braced initializer.
        en = declEnd(f.text.content, oe, false, false);
      }
      return {f.text.pos(b), f.text.pos(std::max(en, oe))};
    };
    auto commentOf = [&](const Range &extent) -> const char * {
      if (!g_config->index.comments)
        return "";
      size_t b = f.text.offset(extent.start.line + 1, 1);
      if (b == std::string::npos)
        return "";
      while (b < f.text.content.size() && (f.text.content[b] == ' ' || f.text.content[b] == '\t'))
        b++;
      return intern(getComment(f.text.content, b, oe));
    };

    auto doDefDecl = [&](auto &entity) {
      Use use{{range, role}, -1};
      if (is_def) {
        Range extent = extentOf();
        entity.def.spell = {use, extent};
        entity.def.parent_kind = SymbolKind(e.parent_kind);
        if (!entity.def.comments[0])
          entity.def.comments = commentOf(extent);
      } else if (is_decl) {
        Range extent = extentOf();
        entity.declarations.push_back({use, extent});
        if (!entity.def.comments[0])
          entity.def.comments = commentOf(extent);
      } else {
        entity.uses.push_back(use);
      }
    };
    // The initializer as written, e.g. " = 1" or "{1}".
    auto initializer = [&]() -> std::string {
      if (!o.init_begin.valid())
        return "";
      size_t b = f.text.offset(o.init_begin.line, o.init_begin.column),
             en = f.text.offset(o.init_end.line, o.init_end.column);
      if (b == std::string::npos || en == std::string::npos || b > en)
        return "";
      StringRef init(f.text.content.data() + b, tokenEnd(f.text.content, en) - b);
      if (init.count('\n') > g_config->index.maxInitializerLines - 1)
        return "";
      if (init.consume_front("="))
        return (" = " + init.ltrim()).str();
      return init.str();
    };

    if (kind == Kind::Func && (is_def || is_decl) && e.parent >= 0 &&
        (e.symbol_kind == (uint8_t)SymbolKind::Constructor || e.short_name[0] == '~')) {
      size_t b = ob + (text[ob] == '~');
      while (b < oe && !isIdentChar(text[b]))
        b++;
      if (b < oe)
        db->toType(param.usr(e.parent)).uses.push_back({{{f.text.pos(b), f.text.pos(oe)}, Role::Reference}, -1});
    }

    switch (kind) {
    case Kind::Func: {
      IndexFunc &func = db->toFunc(usr);
      func.def.kind = SymbolKind(e.symbol_kind);
      func.def.storage = uint8_t(e.storage);
      doDefDecl(func);
      if (func.def.detailed_name[0] == '\0')
        setName(func.def, e);
      if (is_def || is_decl) {
        if (e.parent >= 0 && unit.entities[e.parent].kind == edg::EntityKind::Type)
          db->toType(param.usr(e.parent)).def.funcs.push_back(usr);
        for (int b : e.bases) {
          Usr usr1 = param.usr(b);
          func.def.bases.push_back(usr1);
          db->toFunc(usr1).derived.push_back(usr);
        }
      } else if (o.container >= 0) {
        db->toFunc(param.usr(o.container)).def.callees.push_back({range, usr, Kind::Func, role});
      }
      break;
    }
    case Kind::Type: {
      IndexType &type = db->toType(usr);
      type.def.kind = SymbolKind(e.symbol_kind);
      doDefDecl(type);
      if (type.def.detailed_name[0] == '\0' || is_def)
        setName(type.def, e);
      if (is_def || is_decl) {
        if (e.parent >= 0 && unit.entities[e.parent].kind == edg::EntityKind::Type)
          db->toType(param.usr(e.parent)).def.types.push_back(usr);
      }
      if (is_def || (is_decl && e.symbol_kind == (uint8_t)SymbolKind::Namespace)) {
        for (int b : e.bases) {
          Usr usr1 = param.usr(b);
          type.def.bases.push_back(usr1);
          db->toType(usr1).derived.push_back(usr);
        }
        if (type.def.vars.empty())
          for (auto &[field, offset] : e.fields)
            if (field >= 0)
              type.def.vars.emplace_back(param.usr(field), offset);
      }
      if (e.alias_of >= 0 && (is_def || is_decl)) {
        type.def.alias_of = param.usr(e.alias_of);
        (void)db->toType(type.def.alias_of);
      }
      break;
    }
    case Kind::Var: {
      IndexVar &var = db->toVar(usr);
      var.def.kind = SymbolKind(e.symbol_kind);
      var.def.storage = uint8_t(e.storage);
      doDefDecl(var);
      if (var.def.detailed_name[0] == '\0')
        setName(var.def, e);
      if (is_def || is_decl) {
        if (e.parent >= 0) {
          const edg::Entity &p = unit.entities[e.parent];
          if (p.kind == edg::EntityKind::Func)
            db->toFunc(param.usr(e.parent)).def.vars.push_back(usr);
          else if (p.kind == edg::EntityKind::Type && p.symbol_kind != (uint8_t)SymbolKind::Class &&
                   p.symbol_kind != (uint8_t)SymbolKind::Struct)
            db->toType(param.usr(e.parent)).def.vars.emplace_back(usr, -1);
        }
        if (e.type >= 0) {
          var.def.type = param.usr(e.type);
          if (e.symbol_kind != (uint8_t)SymbolKind::EnumMember)
            db->toType(var.def.type).instances.push_back(usr);
        }
        if (is_def && e.symbol_kind != (uint8_t)SymbolKind::Macro && !var.def.hover[0]) {
          std::string init = initializer();
          if (init.size()) {
            std::string hover = var.def.detailed_name;
            if (var.def.storage == SC_Static && strncmp(var.def.detailed_name, "static ", 7))
              hover = "static " + hover;
            var.def.hover = intern(hover + init);
          }
        }
      }
      if (e.symbol_kind == (uint8_t)SymbolKind::Macro && is_def) {
        const Range &ext = var.def.spell->extent;
        size_t b = f.text.offset(ext.start.line + 1, 1);
        size_t en = f.text.offset(ext.end.line + 1, 1);
        if (b != std::string::npos && en != std::string::npos) {
          size_t eol = f.text.content.find('\n', en);
          if (eol == std::string::npos)
            eol = f.text.content.size();
          StringRef text(f.text.content.data() + b, eol - b);
          text = text.trim();
          var.def.hover = intern(text.count('\n') <= g_config->index.maxInitializerLines - 1
                                     ? text.str()
                                     : ("#define " + e.short_name));
        }
      }
      break;
    }
    default:
      break;
    }
  }

  void addSkippedRanges() {
    for (const edg::SkippedRange &r : unit.skipped) {
      IndexFile *db = param.consumeFile(r.begin.file);
      if (!db)
        continue;
      File &f = param.file(r.begin.file);
      size_t b = f.text.offset(r.begin.line, 1), e = f.text.offset(r.end.line, r.end.column);
      if (b == std::string::npos || e == std::string::npos || b > e)
        continue;
      // Include the directive ending the region, e.g. "#endif".
      const std::string &text = f.text.content;
      if (e < text.size() && text[e] == '#') {
        e++;
        while (e < text.size() && (text[e] == ' ' || text[e] == '\t'))
          e++;
        while (e < text.size() && isIdentChar(text[e]))
          e++;
      }
      Range range{f.text.pos(b), f.text.pos(e)};
      // A file included more than once is reported once per inclusion.
      if (std::find(db->skipped_ranges.begin(), db->skipped_ranges.end(), range) == db->skipped_ranges.end())
        db->skipped_ranges.push_back(range);
    }
  }

  void addIncludes() {
    for (const edg::Include &inc : unit.includes) {
      IndexFile *db = param.consumeFile(inc.file);
      if (!db || inc.line <= 0)
        continue;
      File &child = param.file(inc.included_file);
      const char *path = intern(child.path);
      if (child.path.size() && std::none_of(db->includes.begin(), db->includes.end(), [&](const IndexInclude &i) {
            return i.line == inc.line - 1 && i.resolved_path == path;
          }))
        db->includes.push_back({inc.line - 1, path});
    }
  }
};

template <typename T> void uniquify(std::vector<T> &a) {
  std::unordered_set<T> seen;
  size_t n = 0;
  for (size_t i = 0; i < a.size(); i++)
    if (seen.insert(a[i]).second)
      a[n++] = a[i];
  a.resize(n);
}
} // namespace

const int IndexFile::kMajorVersion = 21;
const int IndexFile::kMinorVersion = 0;

IndexFile::IndexFile(const std::string &path, const std::string &contents, bool no_linkage)
    : path(path), no_linkage(no_linkage), file_contents(contents) {}

IndexFunc &IndexFile::toFunc(Usr usr) {
  auto [it, inserted] = usr2func.try_emplace(usr);
  if (inserted)
    it->second.usr = usr;
  return it->second;
}

IndexType &IndexFile::toType(Usr usr) {
  auto [it, inserted] = usr2type.try_emplace(usr);
  if (inserted)
    it->second.usr = usr;
  return it->second;
}

IndexVar &IndexFile::toVar(Usr usr) {
  auto [it, inserted] = usr2var.try_emplace(usr);
  if (inserted)
    it->second.usr = usr;
  return it->second;
}

std::string IndexFile::toString() { return ccls::serialize(SerializeFormat::Json, *this); }

namespace idx {
void init() {
  multiVersionMatcher = new GroupMatch(g_config->index.multiVersionWhitelist, g_config->index.multiVersionBlacklist);
  if (g_config->edg.baseDir.size())
    edg::setBaseDirectory(g_config->edg.baseDir);
}

IndexResult index(WorkingFiles *wfiles, VFS *vfs, const std::string &opt_wdir, const std::string &main,
                  const std::vector<const char *> &args,
                  const std::vector<std::pair<std::string, std::string>> &remapped, bool no_linkage, bool &ok) {
  ok = true;
  EdgInvocation inv;
  // e.g. .s
  if (!buildEdgInvocation(args, main, inv))
    return {};
  ok = false;
  edg::Request req;
  req.args = std::move(inv.args);
  req.args.insert(req.args.begin(), "--no_warnings");
  req.index = true;
  req.all_linkages = no_linkage;
  if (wfiles->getContent(main).size())
    req.remapped = remapped;

  edg::Unit unit;
  IndexParam param(*vfs, no_linkage, unit, req.remapped);
  req.want_file = [&](int fid, const std::string &) { return param.consumeFile(fid) != nullptr; };
  {
    llvm::CrashRecoveryContext crc;
    if (!crc.RunSafely([&]() { edg::run(req, unit); })) {
      LOG_S(ERROR) << "the front end crashed for " << main;
      llvm::CrashRecoveryContext().RunSafely([]() { edg::recover(); });
      return {};
    }
  }
  if (!unit.completed) {
    std::string reason;
    for (Diag &d : parseSarif(unit.sarif))
      if (d.level >= DiagLevel::Error) {
        reason = d.message;
        break;
      }
    LOG_S(ERROR) << "failed to index " << main << (reason.empty() ? "" : ": " + reason);
    if (LOG_V_ENABLED(1)) {
      std::string line;
      for (auto &arg : req.args)
        (line += ' ') += arg;
      LOG_V(1) << "front end arguments:" << line << "\n" << unit.sarif;
    }
    return {};
  }
  ok = true;

  Indexer indexer(param);
  for (const edg::Occurrence &o : unit.occurrences)
    indexer.addOccurrence(o);
  indexer.addIncludes();
  indexer.addSkippedRanges();
  // The main file is indexed even if it has no symbol.
  for (size_t i = 0; i < unit.files.size(); i++)
    if (param.file((int)i).path == main || unit.files[i].path == main)
      param.consumeFile((int)i);

  IndexResult result;
  result.n_errs = unit.n_errors;
  std::vector<Diag> diags = parseSarif(unit.sarif);
  for (Diag &d : diags)
    if (d.level >= DiagLevel::Error) {
      result.first_error = d.file + ":" + std::to_string(d.range.start.line + 1) + ": " + d.message;
      break;
    }
  for (File &file : param.files) {
    if (!file.db)
      continue;
    std::unique_ptr<IndexFile> &entry = file.db;
    entry->import_file = main;
    entry->args = args;
    entry->language = inv.lang;
    // Instantiating a class template re-declares the members of the template.
    auto dropDefinition = [](auto &entity) {
      if (entity.def.spell)
        entity.declarations.erase(std::remove_if(entity.declarations.begin(), entity.declarations.end(),
                                                 [&](const DeclRef &d) { return d.range == entity.def.spell->range; }),
                                  entity.declarations.end());
    };
    for (auto &it : entry->usr2func)
      dropDefinition(it.second);
    for (auto &it : entry->usr2type)
      dropDefinition(it.second);
    for (auto &it : entry->usr2var)
      dropDefinition(it.second);
    for (auto &it : entry->usr2func) {
      // e.g. declaration + out-of-line definition
      uniquify(it.second.derived);
      uniquify(it.second.uses);
      uniquify(it.second.declarations);
      uniquify(it.second.def.bases);
      uniquify(it.second.def.vars);
      uniquify(it.second.def.callees);
    }
    for (auto &it : entry->usr2type) {
      uniquify(it.second.derived);
      uniquify(it.second.uses);
      uniquify(it.second.declarations);
      uniquify(it.second.instances);
      uniquify(it.second.def.bases);
      uniquify(it.second.def.funcs);
      uniquify(it.second.def.types);
    }
    for (auto &it : entry->usr2var) {
      uniquify(it.second.uses);
      uniquify(it.second.declarations);
    }

    // Update dependencies for the file.
    for (File &f : param.files) {
      if (f.path.empty())
        continue;
      if (f.path == entry->path)
        entry->mtime = f.mtime;
      else if (f.path != entry->import_file)
        entry->dependencies[llvm::CachedHashStringRef(intern(f.path))] = f.mtime;
    }
    result.indexes.push_back(std::move(entry));
  }
  return result;
}
} // namespace idx

void reflect(JsonReader &vis, SymbolRef &v) {
  std::string t = vis.getString();
  char *s = const_cast<char *>(t.c_str());
  v.range = Range::fromString(s);
  s = strchr(s, '|');
  v.usr = strtoull(s + 1, &s, 10);
  v.kind = static_cast<Kind>(strtol(s + 1, &s, 10));
  v.role = static_cast<Role>(strtol(s + 1, &s, 10));
}
void reflect(JsonReader &vis, Use &v) {
  std::string t = vis.getString();
  char *s = const_cast<char *>(t.c_str());
  v.range = Range::fromString(s);
  s = strchr(s, '|');
  v.role = static_cast<Role>(strtol(s + 1, &s, 10));
  v.file_id = static_cast<int>(strtol(s + 1, &s, 10));
}
void reflect(JsonReader &vis, DeclRef &v) {
  std::string t = vis.getString();
  char *s = const_cast<char *>(t.c_str());
  v.range = Range::fromString(s);
  s = strchr(s, '|') + 1;
  v.extent = Range::fromString(s);
  s = strchr(s, '|');
  v.role = static_cast<Role>(strtol(s + 1, &s, 10));
  v.file_id = static_cast<int>(strtol(s + 1, &s, 10));
}

void reflect(JsonWriter &vis, SymbolRef &v) {
  char buf[99];
  snprintf(buf, sizeof buf, "%s|%" PRIu64 "|%d|%d", v.range.toString().c_str(), v.usr, int(v.kind), int(v.role));
  std::string s(buf);
  reflect(vis, s);
}
void reflect(JsonWriter &vis, Use &v) {
  char buf[99];
  snprintf(buf, sizeof buf, "%s|%d|%d", v.range.toString().c_str(), int(v.role), v.file_id);
  std::string s(buf);
  reflect(vis, s);
}
void reflect(JsonWriter &vis, DeclRef &v) {
  char buf[99];
  snprintf(buf, sizeof buf, "%s|%s|%d|%d", v.range.toString().c_str(), v.extent.toString().c_str(), int(v.role),
           v.file_id);
  std::string s(buf);
  reflect(vis, s);
}

void reflect(BinaryReader &vis, SymbolRef &v) {
  reflect(vis, v.range);
  reflect(vis, v.usr);
  reflect(vis, v.kind);
  reflect(vis, v.role);
}
void reflect(BinaryReader &vis, Use &v) {
  reflect(vis, v.range);
  reflect(vis, v.role);
  reflect(vis, v.file_id);
}
void reflect(BinaryReader &vis, DeclRef &v) {
  reflect(vis, static_cast<Use &>(v));
  reflect(vis, v.extent);
}

void reflect(BinaryWriter &vis, SymbolRef &v) {
  reflect(vis, v.range);
  reflect(vis, v.usr);
  reflect(vis, v.kind);
  reflect(vis, v.role);
}
void reflect(BinaryWriter &vis, Use &v) {
  reflect(vis, v.range);
  reflect(vis, v.role);
  reflect(vis, v.file_id);
}
void reflect(BinaryWriter &vis, DeclRef &v) {
  reflect(vis, static_cast<Use &>(v));
  reflect(vis, v.extent);
}
} // namespace ccls
