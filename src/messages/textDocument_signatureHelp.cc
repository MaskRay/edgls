// Copyright 2017-2018 ccls Authors
// SPDX-License-Identifier: Apache-2.0

#include "message_handler.hh"
#include "pipeline.hh"
#include "query.hh"
#include "working_files.hh"

namespace ccls {
namespace {
struct ParameterInformation {
  std::vector<int> label;
};
struct SignatureInformation {
  std::string label;
  std::optional<std::string> documentation;
  std::vector<ParameterInformation> parameters;
};
struct SignatureHelp {
  std::vector<SignatureInformation> signatures;
  int activeSignature = 0;
  int activeParameter = 0;
};
REFLECT_STRUCT(ParameterInformation, label);
REFLECT_STRUCT(SignatureInformation, label, documentation, parameters);
REFLECT_STRUCT(SignatureHelp, signatures, activeSignature, activeParameter);

// Build a signature from a function's detailed name, e.g.
// "int ns::f(int, const char *) const".
SignatureInformation buildSignature(const QueryFunc::Def &def) {
  SignatureInformation sig;
  sig.label = def.detailed_name;
  if (def.comments[0])
    sig.documentation = def.comments;
  size_t i = sig.label.find('(', def.short_name_offset + def.short_name_size);
  if (i == std::string::npos)
    return sig;
  int depth = 0;
  size_t start = i + 1;
  for (size_t j = i + 1; j < sig.label.size(); j++) {
    char c = sig.label[j];
    if (c == '(' || c == '<' || c == '[' || c == '{')
      depth++;
    else if ((c == ')' || c == '>' || c == ']' || c == '}') && depth > 0)
      depth--;
    else if ((c == ',' || c == ')') && depth == 0) {
      size_t b = start, e = j;
      while (b < e && sig.label[b] == ' ')
        b++;
      if (b < e)
        sig.parameters.push_back({{(int)b, (int)e}});
      start = j + 1;
      if (c == ')')
        break;
    }
  }
  if (sig.parameters.size() == 1) {
    auto &l = sig.parameters[0].label;
    if (sig.label.compare(l[0], l[1] - l[0], "void") == 0)
      sig.parameters.clear();
  }
  return sig;
}
} // namespace

void MessageHandler::textDocument_signatureHelp(TextDocumentPositionParam &param, ReplyOnce &reply) {
  std::string path = param.textDocument.uri.getPath();
  WorkingFile *wf = wfiles->getFile(path);
  if (!wf) {
    reply.notOpened(path);
    return;
  }
  SignatureHelp result;
  QueryFile *file = findFile(path);
  const std::string &content = wf->buffer_content;
  int off = getOffsetForPosition(param.position, content);
  if (!file || off < 0) {
    reply(result);
    return;
  }

  // Find the unmatched '(' before the cursor and count the arguments.
  int depth = 0, args = 0, open = -1;
  for (int i = std::min<int>(off, content.size()) - 1; i >= 0 && off - i < 4096; i--) {
    char c = content[i];
    if (c == ')' || c == ']' || c == '}')
      depth++;
    else if (c == '(' || c == '[' || c == '{') {
      if (depth-- == 0) {
        if (c == '(')
          open = i;
        break;
      }
    } else if (c == ',' && depth == 0)
      args++;
    else if (c == ';')
      break;
  }
  if (open < 0) {
    reply(result);
    return;
  }
  int name_end = open;
  while (name_end > 0 && isspace((unsigned char)content[name_end - 1]))
    name_end--;
  // Skip explicit template arguments: f<int>(
  if (name_end > 0 && content[name_end - 1] == '>') {
    int d = 0;
    for (int i = name_end - 1; i >= 0; i--) {
      if (content[i] == '>')
        d++;
      else if (content[i] == '<' && --d == 0) {
        name_end = i;
        break;
      }
    }
  }
  if (name_end == 0 || !isAsciiIdentifierContinue(content[name_end - 1])) {
    reply(result);
    return;
  }
  Position name_pos = getPositionForOffset(content, name_end - 1);

  std::vector<SymbolIdx> syms;
  for (SymbolRef sym : findSymbolsAtLocation(wf, file, name_pos))
    syms.push_back(sym);
  if (syms.empty()) {
    // The call may be on a line being edited.
    int b = name_end;
    while (b > 0 && isAsciiIdentifierContinue(content[b - 1]))
      b--;
    int q = b;
    while (q >= 2 && content.compare(q - 2, 2, "::") == 0) {
      int r = q - 2;
      while (r > 0 && isAsciiIdentifierContinue(content[r - 1]))
        r--;
      if (r == q - 2)
        break;
      q = r;
    }
    std::string_view name(content.data() + b, name_end - b);
    std::string_view qualifier = q < b ? std::string_view(content.data() + q, b - 2 - q) : std::string_view();
    int col = param.position.character;
    std::optional<int> line = wf->getIndexPosFromBufferPos(param.position.line, &col, false);
    syms = lookupName(db, file, {(uint16_t)(line ? *line : param.position.line), (int16_t)col}, name, qualifier);
  }

  std::vector<const QueryFunc::Def *> cands;
  for (SymbolIdx sym : syms) {
    if (sym.kind == Kind::Func && db->hasFunc(sym.usr)) {
      if (auto *def = db->getFunc(sym.usr).anyDef()) {
        std::string_view qualified = def->name(true);
        // Overloads with the same qualified name.
        for (QueryFunc &func : db->funcs)
          if (auto *def1 = func.anyDef(); def1 && def1->name(true) == qualified)
            cands.push_back(def1);
      }
    } else if (sym.kind == Kind::Type && db->hasType(sym.usr)) {
      // Constructors.
      for (auto &tdef : db->getType(sym.usr).def)
        for (Usr usr : tdef.funcs)
          if (db->hasFunc(usr))
            if (auto *def = db->getFunc(usr).anyDef(); def && def->kind == SymbolKind::Constructor)
              cands.push_back(def);
    }
    if (cands.size())
      break;
  }
  std::sort(cands.begin(), cands.end());
  cands.erase(std::unique(cands.begin(), cands.end()), cands.end());
  for (auto *def : cands)
    result.signatures.push_back(buildSignature(*def));
  std::sort(result.signatures.begin(), result.signatures.end(),
            [](const SignatureInformation &l, const SignatureInformation &r) {
              if (l.parameters.size() != r.parameters.size())
                return l.parameters.size() < r.parameters.size();
              return l.label < r.label;
            });
  result.activeParameter = args;
  for (size_t i = 0; i < result.signatures.size(); i++)
    if ((int)result.signatures[i].parameters.size() > args) {
      result.activeSignature = (int)i;
      break;
    }
  reply(result);
}
} // namespace ccls
