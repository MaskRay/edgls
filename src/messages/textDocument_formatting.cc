// Copyright 2017-2018 ccls Authors
// SPDX-License-Identifier: Apache-2.0

#include "message_handler.hh"
#include "pipeline.hh"
#include "working_files.hh"

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/Program.h>

namespace ccls {
using namespace llvm;

namespace {
struct Replacement {
  unsigned offset, length;
  std::string text;
};

std::string unescapeXml(StringRef s) {
  std::string ret;
  for (size_t i = 0; i < s.size(); i++) {
    if (s[i] != '&') {
      ret += s[i];
      continue;
    }
    size_t semi = s.find(';', i);
    if (semi == StringRef::npos) {
      ret += s[i];
      continue;
    }
    StringRef ent = s.slice(i + 1, semi);
    unsigned v;
    if (ent == "lt")
      ret += '<';
    else if (ent == "gt")
      ret += '>';
    else if (ent == "amp")
      ret += '&';
    else if (ent == "apos")
      ret += '\'';
    else if (ent == "quot")
      ret += '"';
    else if (ent.consume_front("#") && !ent.getAsInteger(10, v))
      ret += char(v);
    else
      ret += s.slice(i, semi + 1);
    i = semi;
  }
  return ret;
}

// Run clang-format on |code| and return the replacements.
Expected<std::vector<Replacement>> formatCode(StringRef code, StringRef file, unsigned offset, unsigned length) {
  auto program = sys::findProgramByName("clang-format");
  if (!program)
    return createStringError(inconvertibleErrorCode(), "clang-format not found");
  SmallString<128> in, out;
  if (std::error_code ec = sys::fs::createTemporaryFile("edgls-format", "in", in))
    return createStringError(ec, "cannot create temporary file");
  if (std::error_code ec = sys::fs::createTemporaryFile("edgls-format", "out", out)) {
    sys::fs::remove(in);
    return createStringError(ec, "cannot create temporary file");
  }
  writeToFile(std::string(in), code.str());
  std::string assume = ("--assume-filename=" + file).str(), off = "--offset=" + std::to_string(offset),
              len = "--length=" + std::to_string(length);
  std::vector<StringRef> argv{*program, "--style=file", "--fallback-style=LLVM", "--output-replacements-xml",
                              assume,   off,            len};
  std::optional<StringRef> redirects[3] = {StringRef(in), StringRef(out), StringRef("")};
  std::string err;
  int rc = sys::ExecuteAndWait(*program, argv, std::nullopt, redirects, 10, 0, &err);
  auto buf = MemoryBuffer::getFile(out);
  sys::fs::remove(in);
  sys::fs::remove(out);
  if (rc != 0 || !buf)
    return createStringError(inconvertibleErrorCode(), "clang-format failed" + (err.empty() ? "" : ": " + err));

  std::vector<Replacement> ret;
  StringRef xml = (*buf)->getBuffer();
  for (size_t i = 0; (i = xml.find("<replacement ", i)) != StringRef::npos;) {
    size_t close = xml.find('>', i), end = xml.find("</replacement>", i);
    if (close == StringRef::npos || end == StringRef::npos)
      break;
    StringRef attrs = xml.slice(i, close);
    auto attr = [&](StringRef name) {
      unsigned v = 0;
      size_t p = attrs.find(name);
      if (p != StringRef::npos) {
        StringRef rest = attrs.substr(p + name.size()).drop_while([](char c) { return c == '=' || c == '\''; });
        rest.take_while([](char c) { return isdigit((unsigned char)c); }).getAsInteger(10, v);
      }
      return v;
    };
    ret.push_back({attr("offset"), attr("length"), unescapeXml(xml.slice(close + 1, end))});
    i = end;
  }
  return ret;
}

std::vector<TextEdit> replacementsToEdits(std::string_view code, const std::vector<Replacement> &repls) {
  std::vector<TextEdit> ret;
  int i = 0, line = 0, col = 0;
  auto move = [&](unsigned p) {
    for (; i < (int)p; i++)
      if (code[i] == '\n')
        line++, col = 0;
      else {
        if ((uint8_t)code[i] >= 128) {
          while (128 <= (uint8_t)code[++i] && (uint8_t)code[i] < 192)
            ;
          i--;
        }
        col++;
      }
  };
  for (const auto &r : repls) {
    move(r.offset);
    int l = line, c = col;
    move(r.offset + r.length);
    ret.push_back({{{l, c}, {line, col}}, r.text});
  }
  return ret;
}

void format(ReplyOnce &reply, WorkingFile *wfile, unsigned offset, unsigned length) {
  std::string_view code = wfile->buffer_content;
  auto replsOrErr = formatCode(StringRef(code.data(), code.size()),
                               StringRef(wfile->filename.data(), wfile->filename.size()), offset, length);
  if (replsOrErr)
    reply(replacementsToEdits(code, *replsOrErr));
  else
    reply.error(ErrorCode::UnknownErrorCode, llvm::toString(replsOrErr.takeError()));
}
} // namespace

void MessageHandler::textDocument_formatting(DocumentFormattingParam &param, ReplyOnce &reply) {
  auto [file, wf] = findOrFail(param.textDocument.uri.getPath(), reply);
  if (!wf)
    return;
  format(reply, wf, 0, (unsigned)wf->buffer_content.size());
}

void MessageHandler::textDocument_onTypeFormatting(DocumentOnTypeFormattingParam &param, ReplyOnce &reply) {
  auto [file, wf] = findOrFail(param.textDocument.uri.getPath(), reply);
  if (!wf) {
    return;
  }
  std::string_view code = wf->buffer_content;
  int pos = getOffsetForPosition(param.position, code);
  auto lbrace = code.find_last_of('{', pos);
  if (lbrace == std::string::npos)
    lbrace = pos;
  format(reply, wf, (unsigned)lbrace, unsigned(pos - lbrace));
}

void MessageHandler::textDocument_rangeFormatting(DocumentRangeFormattingParam &param, ReplyOnce &reply) {
  auto [file, wf] = findOrFail(param.textDocument.uri.getPath(), reply);
  if (!wf) {
    return;
  }
  std::string_view code = wf->buffer_content;
  int begin = getOffsetForPosition(param.range.start, code), end = getOffsetForPosition(param.range.end, code);
  if (end < begin)
    std::swap(begin, end);
  format(reply, wf, (unsigned)begin, unsigned(end - begin));
}
} // namespace ccls
