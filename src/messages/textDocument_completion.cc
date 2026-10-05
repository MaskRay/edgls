// Copyright 2017-2018 ccls Authors
// SPDX-License-Identifier: Apache-2.0

#include "fuzzy_match.hh"
#include "log.hh"
#include "message_handler.hh"
#include "pipeline.hh"
#include "query.hh"
#include "sema_manager.hh"
#include "working_files.hh"

#include <llvm/ADT/DenseSet.h>
#include <llvm/ADT/StringRef.h>

#include <unordered_set>

namespace ccls {
using namespace llvm;

REFLECT_UNDERLYING(InsertTextFormat);
REFLECT_UNDERLYING(CompletionItemKind);

void reflect(JsonWriter &vis, CompletionItem &v) {
  reflectMemberStart(vis);
  REFLECT_MEMBER(label);
  REFLECT_MEMBER(kind);
  REFLECT_MEMBER(detail);
  if (v.documentation.size())
    REFLECT_MEMBER(documentation);
  REFLECT_MEMBER(sortText);
  if (v.filterText.size())
    REFLECT_MEMBER(filterText);
  REFLECT_MEMBER(insertTextFormat);
  REFLECT_MEMBER(textEdit);
  if (v.additionalTextEdits.size())
    REFLECT_MEMBER(additionalTextEdits);
  reflectMemberEnd(vis);
}

namespace {
struct CompletionList {
  bool isIncomplete = false;
  std::vector<CompletionItem> items;
};
REFLECT_STRUCT(CompletionList, isIncomplete, items);

// Pre-filters completion responses before sending to vscode. This results in a
// significantly snappier completion experience as vscode is easily overloaded
// when given 1000+ completion items.
void filterCandidates(CompletionList &result, const std::string &complete_text, Position begin_pos, Position end_pos,
                      const std::string &buffer_line) {
  assert(begin_pos.line == end_pos.line);
  auto &items = result.items;

  // People usually does not want to insert snippets or parenthesis when
  // changing function or type names, e.g. "str.|()" or "std::|<int>".
  bool has_open_paren = false;
  for (int c = end_pos.character; c < buffer_line.size(); ++c) {
    if (buffer_line[c] == '(' || buffer_line[c] == '<')
      has_open_paren = true;
    if (!isspace(buffer_line[c]))
      break;
  }

  auto finalize = [&]() {
    int max_num = g_config->completion.maxNum;
    if (items.size() > max_num) {
      items.resize(max_num);
      result.isIncomplete = true;
    }

    std::string sort(4, ' ');
    for (auto &item : items) {
      item.textEdit.range = lsRange{begin_pos, end_pos};
      if (has_open_paren) {
        item.textEdit.newText = item.filterText;
        item.insertTextFormat = InsertTextFormat::PlainText;
      }
      if (item.filterText == item.label)
        item.filterText.clear();
      for (auto i = sort.size(); i && ++sort[i - 1] == 'A';)
        sort[--i] = ' ';
      item.sortText = sort;
    }
  };

  if (!g_config->completion.filterAndSort) {
    finalize();
    return;
  }

  if (complete_text.size()) {
    // Fuzzy match and remove awful candidates.
    bool sensitive = g_config->completion.caseSensitivity;
    FuzzyMatcher fuzzy(complete_text, sensitive);
    for (CompletionItem &item : items) {
      const std::string &filter = item.filterText.size() ? item.filterText : item.label;
      item.score_ = reverseSubseqMatch(complete_text, filter, sensitive) >= 0 ? fuzzy.match(filter, true)
                                                                              : FuzzyMatcher::kMinScore;
    }
    items.erase(std::remove_if(items.begin(), items.end(),
                               [](const CompletionItem &item) { return item.score_ <= FuzzyMatcher::kMinScore; }),
                items.end());
  }
  std::sort(items.begin(), items.end(), [](const CompletionItem &lhs, const CompletionItem &rhs) {
    if (lhs.score_ != rhs.score_)
      return lhs.score_ > rhs.score_;
    if (lhs.priority_ != rhs.priority_)
      return lhs.priority_ < rhs.priority_;
    int t = lhs.textEdit.newText.compare(rhs.textEdit.newText);
    if (t)
      return t < 0;
    t = lhs.label.compare(rhs.label);
    if (t)
      return t < 0;
    return lhs.filterText < rhs.filterText;
  });

  // Trim result.
  finalize();
}

CompletionItemKind toCompletionKind(SymbolKind k) {
  switch (k) {
  case SymbolKind::Namespace:
    return CompletionItemKind::Module;
  case SymbolKind::Class:
    return CompletionItemKind::Class;
  case SymbolKind::Struct:
    return CompletionItemKind::Struct;
  case SymbolKind::Enum:
    return CompletionItemKind::Enum;
  case SymbolKind::Interface:
    return CompletionItemKind::Interface;
  case SymbolKind::TypeAlias:
  case SymbolKind::TypeParameter:
    return CompletionItemKind::TypeParameter;
  case SymbolKind::Method:
  case SymbolKind::StaticMethod:
    return CompletionItemKind::Method;
  case SymbolKind::Constructor:
    return CompletionItemKind::Constructor;
  case SymbolKind::Function:
    return CompletionItemKind::Function;
  case SymbolKind::Field:
    return CompletionItemKind::Field;
  case SymbolKind::EnumMember:
    return CompletionItemKind::EnumMember;
  case SymbolKind::Macro:
    return CompletionItemKind::Reference;
  default:
    return CompletionItemKind::Variable;
  }
}

const char *const kKeywords[] = {
    "alignas",   "alignof",  "auto",          "bool",        "break",     "case",      "catch",
    "char",      "class",    "concept",       "const",       "consteval", "constexpr", "constinit",
    "continue",  "co_await", "co_return",     "co_yield",    "decltype",  "default",   "delete",
    "do",        "double",   "dynamic_cast",  "else",        "enum",      "explicit",  "extern",
    "false",     "float",    "for",           "friend",      "goto",      "if",        "inline",
    "int",       "long",     "mutable",       "namespace",   "new",       "noexcept",  "nullptr",
    "operator",  "private",  "protected",     "public",      "register",  "reinterpret_cast",
    "requires",  "return",   "short",         "signed",      "sizeof",    "static",    "static_assert",
    "static_cast", "struct", "switch",        "template",    "this",      "thread_local", "throw",
    "true",      "try",      "typedef",       "typeid",      "typename",  "union",     "unsigned",
    "using",     "virtual",  "void",          "volatile",    "while",
};

// Collects candidates from the index.
struct Collector {
  DB *db;
  std::vector<CompletionItem> items;
  llvm::DenseSet<std::pair<Usr, int>> seen;
  // Labels of added items; a derived class member hides a base class member.
  std::unordered_set<std::string> labels;

  template <typename Def> void add(Usr usr, Kind kind, const Def &def, unsigned priority) {
    if (!seen.insert({usr, int(kind)}).second)
      return;
    std::string_view name = def.name(false);
    if (name.empty() || name[0] == '~' || (name.size() > 8 && name.substr(0, 8) == "operator" &&
                                           !isAsciiIdentifierContinue(name[8])))
      return;
    CompletionItem item;
    item.label = std::string(name);
    item.filterText = item.label;
    item.kind = toCompletionKind(def.kind);
    item.detail = def.detailed_name;
    item.documentation = def.comments;
    item.textEdit.newText = item.label;
    if (kind == Kind::Func && g_config->client.snippetSupport) {
      item.textEdit.newText += "($1)$0";
      item.insertTextFormat = InsertTextFormat::Snippet;
    }
    if (g_config->completion.detailedLabel && kind == Kind::Func) {
      std::string_view detailed = def.detailed_name;
      size_t paren = detailed.find('(', def.short_name_offset);
      if (paren != std::string_view::npos) {
        detailed = detailed.substr(paren);
        // An overrider hides the overridden function.
        for (std::string_view suffix : {" = 0", " final", " override"})
          if (detailed.size() > suffix.size() && detailed.substr(detailed.size() - suffix.size()) == suffix)
            detailed.remove_suffix(suffix.size());
        item.label += detailed;
      }
    }
    if (!labels.insert(item.label).second)
      return;
    item.priority_ = priority;
    item.score_ = 0;
    items.push_back(std::move(item));
  }

  void addFunc(Usr usr, unsigned priority) {
    if (!db->hasFunc(usr))
      return;
    if (auto *def = db->getFunc(usr).anyDef())
      if (def->kind != SymbolKind::Constructor)
        add(usr, Kind::Func, *def, priority);
  }
  void addType(Usr usr, unsigned priority) {
    if (!db->hasType(usr))
      return;
    if (auto *def = db->getType(usr).anyDef())
      add(usr, Kind::Type, *def, priority);
  }
  void addVar(Usr usr, unsigned priority) {
    if (!db->hasVar(usr))
      return;
    if (auto *def = db->getVar(usr).anyDef())
      add(usr, Kind::Var, *def, priority);
  }

  // Members of a class or namespace, including those of base classes.
  void addMembers(Usr type_usr, bool types_too, unsigned priority, int depth = 0) {
    if (depth > 8 || !db->hasType(type_usr))
      return;
    QueryType &type = db->getType(type_usr);
    for (auto &def : type.def) {
      for (Usr usr : def.funcs)
        addFunc(usr, priority);
      for (auto &[usr, _] : def.vars)
        addVar(usr, priority);
      if (types_too)
        for (Usr usr : def.types)
          addType(usr, priority);
      if (def.kind != SymbolKind::Namespace)
        for (Usr base : def.bases)
          addMembers(base, types_too, priority + 1, depth + 1);
      if (def.alias_of)
        addMembers(def.alias_of, types_too, priority, depth + 1);
    }
  }

  // The type of a variable, following typedefs to the class.
  Usr typeOfVar(Usr var_usr) {
    if (!db->hasVar(var_usr))
      return 0;
    if (auto *def = db->getVar(var_usr).anyDef())
      return def->type;
    return 0;
  }
};

Pos indexPos(WorkingFile *wf, Position p) {
  int col = p.character;
  std::optional<int> line = wf->getIndexPosFromBufferPos(p.line, &col, false);
  return {(uint16_t)(line ? *line : p.line), (int16_t)col};
}

// The qualifier before the identifier ending at |end|, e.g. "std::chrono" for
// "std::chrono::seconds".
std::string_view qualifierBefore(std::string_view line, size_t begin) {
  size_t i = begin;
  while (i >= 2 && line.substr(i - 2, 2) == "::") {
    size_t j = i - 2;
    while (j > 0 && isAsciiIdentifierContinue(line[j - 1]))
      j--;
    if (j == i - 2)
      break;
    i = j;
  }
  return i < begin ? line.substr(i, begin - 2 - i) : std::string_view();
}
} // namespace

void MessageHandler::textDocument_completion(CompletionParam &param, ReplyOnce &reply) {
  static CompleteConsumerCache<std::vector<CompletionItem>> cache;
  std::string path = param.textDocument.uri.getPath();
  WorkingFile *wf = wfiles->getFile(path);
  if (!wf) {
    reply.notOpened(path);
    return;
  }
  QueryFile *file = findFile(path);

  CompletionList result;

  // It shouldn't be possible, but sometimes vscode will send queries out
  // of order, ie, we get completion request before buffer content update.
  std::string buffer_line;
  if (param.position.line >= 0 && param.position.line < wf->buffer_lines.size())
    buffer_line = wf->buffer_lines[param.position.line];
  bool preprocessor = StringRef(buffer_line).ltrim().starts_with("#");

  if (param.context.triggerKind == CompletionTriggerKind::TriggerCharacter && param.context.triggerCharacter) {
    bool ok = true;
    int col = std::min<int>(param.position.character, buffer_line.size()) - 2;
    switch ((*param.context.triggerCharacter)[0]) {
    case '"':
    case '/':
    case '<':
      ok = false;
      break;
    case ':':
      ok = col >= 0 && buffer_line[col] == ':'; // ::
      break;
    case '>':
      ok = col >= 0 && buffer_line[col] == '-'; // ->
      break;
    }
    if (!ok) {
      reply(result);
      return;
    }
  }

  std::string filter;
  Position end_pos = param.position;
  Position begin_pos = wf->getCompletionPosition(param.position, &filter);

  if (cache.isCacheValid(path, buffer_line, begin_pos)) {
    cache.withLock([&]() { result.items = cache.result; });
    filterCandidates(result, filter, begin_pos, end_pos, buffer_line);
    reply(result);
    return;
  }

  Collector c{db};
  // Classify the context: "x.", "x->", "X::" or unqualified.
  int col = std::min<int>(begin_pos.character, buffer_line.size());
  while (col > 0 && isspace((unsigned char)buffer_line[col - 1]))
    col--;
  StringRef before(buffer_line.data(), std::min<size_t>(col, buffer_line.size()));
  bool member = before.ends_with(".") || before.ends_with("->"), scope = before.ends_with("::");
  if (file && (member || scope)) {
    int base_end = col - (before.ends_with(".") ? 1 : 2);
    while (base_end > 0 && isspace((unsigned char)buffer_line[base_end - 1]))
      base_end--;
    // Skip a call or subscript: f(x). or a[i].
    int depth = 0;
    for (; base_end > 0; base_end--) {
      char ch = buffer_line[base_end - 1];
      if (ch == ')' || ch == ']')
        depth++;
      else if (ch == '(' || ch == '[') {
        if (--depth < 0)
          break;
      } else if (depth == 0)
        break;
    }
    int base_begin = base_end;
    while (base_begin > 0 && isAsciiIdentifierContinue(buffer_line[base_begin - 1]))
      base_begin--;
    std::string_view base_word(buffer_line.data() + base_begin, base_end - base_begin);
    Pos ipos = indexPos(wf, param.position);
    if (member && base_word == "this") {
      if (Usr cls = findEnclosingFunction(db, file, ipos).second)
        c.addMembers(cls, false, 0);
    } else if (base_word.size()) {
      std::vector<SymbolIdx> syms;
      Position base_pos{param.position.line, base_end - 1};
      for (SymbolRef sym : findSymbolsAtLocation(wf, file, base_pos))
        syms.push_back(sym);
      if (syms.empty())
        syms = lookupName(db, file, ipos, base_word, qualifierBefore(buffer_line, base_begin));
      for (SymbolIdx sym : syms) {
        if (sym.kind == Kind::Var) {
          if (Usr t = c.typeOfVar(sym.usr))
            c.addMembers(t, scope, 0);
        } else if (sym.kind == Kind::Type) {
          c.addMembers(sym.usr, true, 0);
        } else if (sym.kind == Kind::Func && db->hasFunc(sym.usr)) {
          // Guess the return type from the declaration.
          if (auto *def = db->getFunc(sym.usr).anyDef()) {
            StringRef ret = StringRef(def->detailed_name, def->qual_name_offset).trim();
            ret.consume_front("virtual ");
            ret.consume_front("static ");
            ret = ret.rtrim(" *&").trim();
            ret.consume_front("const ");
            for (QueryType &type : db->types)
              if (auto *tdef = type.anyDef(); tdef && StringRef(tdef->name(true).data(), tdef->name(true).size()) == ret) {
                c.addMembers(type.usr, false, 0);
                break;
              }
          }
        }
        if (c.items.size())
          break;
      }
    }
  } else if (!preprocessor) {
    if (file) {
      auto [func, cls] = findEnclosingFunction(db, file, indexPos(wf, param.position));
      if (func && db->hasFunc(func))
        for (auto &def : db->getFunc(func).def)
          for (Usr usr : def.vars)
            c.addVar(usr, 0);
      if (cls)
        c.addMembers(cls, true, 1);
    }
    if (filter.size()) {
      // Global symbols.
      for (QueryFunc &func : db->funcs)
        if (auto *def = func.anyDef(); def && def->parent_kind != SymbolKind::Class &&
                                       def->parent_kind != SymbolKind::Struct)
          if (def->name(false).size() && toupper(def->name(false)[0]) == toupper(filter[0]))
            c.add(func.usr, Kind::Func, *def, 2);
      for (QueryType &type : db->types)
        if (auto *def = type.anyDef(); def && def->kind != SymbolKind::TypeParameter && def->name(false).size())
          if (toupper(def->name(false)[0]) == toupper(filter[0]))
            c.add(type.usr, Kind::Type, *def, 2);
      for (QueryVar &var : db->vars)
        if (auto *def = var.anyDef(); def && !def->is_local() && def->kind != SymbolKind::Field &&
                                      def->kind != SymbolKind::Parameter && def->name(false).size())
          if (toupper(def->name(false)[0]) == toupper(filter[0]))
            c.add(var.usr, Kind::Var, *def, 2);
      for (const char *kw : kKeywords) {
        CompletionItem item;
        item.label = item.filterText = item.textEdit.newText = kw;
        item.kind = CompletionItemKind::Keyword;
        item.priority_ = 3;
        item.score_ = 0;
        c.items.push_back(std::move(item));
      }
    }
  }

  result.items = c.items;
  cache.withLock([&]() {
    cache.path = path;
    cache.line = buffer_line;
    cache.position = begin_pos;
    cache.result = c.items;
  });
  filterCandidates(result, filter, begin_pos, end_pos, buffer_line);
  reply(result);
}
} // namespace ccls
