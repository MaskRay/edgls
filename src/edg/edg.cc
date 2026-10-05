// Copyright 2026 edgls Authors
// SPDX-License-Identifier: Apache-2.0

#include "edg.hh"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits.h>
#include <memory>
#include <stdlib.h>
#include <string_view>
#include <unordered_map>

#include "fe_common.h"
#include "fe_wrapup.h"
#include "il_to_str.h"
#include "preproc.h"
#include "scope_stk.h"
#include "symbol_ref.h"
#include "templates.h"

namespace ccls::edg {
namespace E = ::edg;
using namespace ::edg;
namespace {
using EK = EntityKind;

// LSP SymbolKind values.
enum : uint8_t {
  SK_Unknown = 0,
  SK_File = 1,
  SK_Namespace = 3,
  SK_Class = 5,
  SK_Method = 6,
  SK_Field = 8,
  SK_Constructor = 9,
  SK_Enum = 10,
  SK_Interface = 11,
  SK_Function = 12,
  SK_Variable = 13,
  SK_EnumMember = 22,
  SK_Struct = 23,
  SK_TypeParameter = 26,
  SK_TypeAlias = 252,
  SK_Parameter = 253,
  SK_StaticMethod = 254,
  SK_Macro = 255,
};

std::string g_base_dir;

struct SymKey {
  const void *sym;
  unsigned long seq;
  bool operator==(const SymKey &o) const { return sym == o.sym && seq == o.seq; }
};
struct SymKeyHash {
  size_t operator()(const SymKey &k) const { return std::hash<const void *>()(k.sym) ^ (k.seq * 0x9e3779b97f4a7c15ULL); }
};

// A referenced entity as first seen. Entities in file scope memory are
// canonicalized (template instances mapped to templates) and described at the
// end of the translation unit, when the IL is complete. Function-local
// entities are described immediately because their memory may be reused.
struct Raw {
  const void *il = nullptr;
  E::an_il_entry_kind iek = E::iek_none;
  int entity = -1;
};

// A declaration whose extent is computed at the end of the translation unit.
struct PendingExtent {
  size_t occ;
  const void *il;
  E::an_il_entry_kind iek;
  // For a function-local variable, the function whose memory holds it.
  E::a_routine_ptr routine;
};

struct MemStream {
  FILE *file = nullptr;
  char *buf = nullptr;
  size_t size = 0;
};

struct Ctx {
  const Request *req;
  Unit *unit;
  // Output streams handed to the front end. The front end closes f_error
  // during cleanup and then requests a new one.
  std::vector<std::unique_ptr<MemStream>> streams;
  std::unordered_map<std::string, const std::string *> remapped;
  std::unordered_map<E::a_source_file_ptr, int> file_ids;

  std::vector<Raw> raws;
  std::unordered_map<const void *, int> il2raw;
  std::unordered_map<SymKey, int, SymKeyHash> sym2raw;
  std::unordered_map<const void *, int> canon2entity;
  std::vector<PendingExtent> extents;
  // Types of function-local entities and template parameters.
  std::unordered_map<const void *, int> type2raw;
  bool finalizing = false;
  // The routine of the latest declaration, the owner of following parameter
  // declarations, and parameters declared before their routine.
  int last_routine = -1;
  std::vector<int> orphan_params;

  std::string out;
  E::an_il_to_str_output_control_block octl;
  E::a_source_file_ptr last_sf = nullptr;
  int last_file = -1;
  // Per file: 0 unknown, 1 wanted, 2 not wanted.
  std::vector<uint8_t> wanted;
};
thread_local Ctx *ctx;

void outputStr(E::a_const_char *str, E::an_il_to_str_output_control_block_ptr) { ctx->out += str; }

void initOctl() {
  E::clear_il_to_str_output_control_block(&ctx->octl);
  ctx->octl.output_str = outputStr;
  ctx->octl.for_diagnostics = TRUE;
  ctx->octl.render_auto_deduction_typerefs = TRUE;
}

int fileId(E::a_source_file_ptr sf) {
  auto [it, inserted] = ctx->file_ids.try_emplace(sf, (int)ctx->unit->files.size());
  if (inserted) {
    File f;
    f.path = sf->full_name ? sf->full_name : sf->file_name ? sf->file_name : "";
    f.system = sf->from_system_include_dir || sf->included_by_system_include;
    ctx->unit->files.push_back(std::move(f));
  }
  return it->second;
}

Loc toLoc(E::a_seq_number seq, int column) {
  Loc loc;
  if (seq == 0 || column <= 0)
    return loc;
  E::a_const_char *file_name, *full_name;
  E::a_line_number line;
  E::a_boolean at_end;
  E::a_source_file_ptr sf = E::conv_seq_to_file_and_line(seq, &file_name, &full_name, &line, &at_end);
  if (!sf || at_end)
    return loc;
  if (sf != ctx->last_sf) {
    ctx->last_sf = sf;
    ctx->last_file = fileId(sf);
  }
  loc.file = ctx->last_file;
  loc.line = (int)line;
  loc.column = column;
  return loc;
}

// Where the text at |pos| was spelled: the macro argument or definition for
// text produced by a macro expansion.
Loc spellingLoc(const E::a_source_position &pos) {
#if FULLY_RESOLVED_MACRO_POSITIONS
  if (pos.orig_seq != 0 && (pos.orig_seq != pos.seq || pos.orig_column != pos.column))
    return toLoc(pos.orig_seq, pos.orig_column);
#endif
  return toLoc(pos.seq, pos.column);
}

Loc expansionLoc(const E::a_source_position &pos) { return toLoc(pos.seq, pos.column); }

std::string takeOut() {
  std::string ret;
  ret.swap(ctx->out);
  return ret;
}

// Types in unnamed namespaces are printed with a "<unnamed>::" qualifier.
std::string takeType() {
  std::string ret = takeOut();
  for (size_t i; (i = ret.find("<unnamed>::")) != std::string::npos;)
    ret.erase(i, 11);
  return ret;
}

std::string typeToString(E::a_type_ptr type) {
  if (!type)
    return "";
  ctx->out.clear();
  E::form_type(type, &ctx->octl);
  return takeType();
}

// The parts of a declarator of |type| before and after the name.
std::pair<std::string, std::string> declaratorParts(E::a_type_ptr type) {
  ctx->out.clear();
  E::form_type_first_part_simple(type, FALSE, TRUE, &ctx->octl);
  std::string prefix = takeType();
  if (prefix.size() && prefix.back() != ' ' && prefix.back() != '*' && prefix.back() != '&' && prefix.back() != '(')
    prefix += ' ';
  E::form_type_second_part_simple(type, FALSE, &ctx->octl);
  return {prefix, takeType()};
}

E::a_routine_type_supplement_ptr routineTypeInfo(E::a_type_ptr type) {
  for (int i = 0; type && i < 16 && type->kind == E::tk_typeref; i++)
    type = type->variant.typeref.type;
  return type && type->kind == E::tk_routine ? type->variant.routine.extra_info : nullptr;
}

// Like declaratorParts, but the parameter list includes parameter names.
std::pair<std::string, std::string> routineParts(E::a_type_ptr type) {
  auto parts = declaratorParts(type);
  E::a_routine_type_supplement_ptr info = routineTypeInfo(type);
  std::string &suffix = parts.second;
  if (!info || suffix.empty() || suffix[0] != '(')
    return parts;
  int depth = 0;
  size_t close = std::string::npos;
  for (size_t i = 0; i < suffix.size(); i++) {
    if (suffix[i] == '(')
      depth++;
    else if (suffix[i] == ')' && --depth == 0) {
      close = i;
      break;
    }
  }
  if (close == std::string::npos)
    return parts;
  std::string params;
  for (E::a_param_type_ptr p = info->param_type_list; p; p = p->next) {
    if (params.size())
      params += ", ";
    E::a_type_ptr pt = p->declared_type ? p->declared_type : p->type;
    if (p->name && pt) {
      auto [pre, post] = declaratorParts(pt);
      params += pre + (p->is_parameter_pack ? "..." : "") + p->name + post;
    } else {
      params += typeToString(pt);
    }
  }
  if (info->has_ellipsis)
    params += params.empty() ? "..." : ", ...";
  if (params.empty())
    params = suffix.substr(1, close - 1);
  suffix = "(" + params + suffix.substr(close);
  return parts;
}

std::string symbolName(E::a_symbol_ptr sym) {
  ctx->out.clear();
  E::form_symbol_name(sym, &ctx->octl);
  return takeOut();
}

std::string ilName(E::a_source_correspondence *scp, E::an_il_entry_kind kind) {
  ctx->out.clear();
  E::form_name(scp, kind, &ctx->octl);
  return takeOut();
}

E::a_source_correspondence *scpOf(const void *il, E::an_il_entry_kind iek) {
  switch (iek) {
  case E::iek_type:
    return &((E::a_type_ptr)il)->source_corresp;
  case E::iek_routine:
    return &((E::a_routine_ptr)il)->source_corresp;
  case E::iek_variable:
    return &((E::a_variable_ptr)il)->source_corresp;
  case E::iek_field:
    return &((E::a_field_ptr)il)->source_corresp;
  case E::iek_constant:
    return &((E::a_constant_ptr)il)->source_corresp;
  case E::iek_namespace:
    return &((E::a_namespace_ptr)il)->source_corresp;
  case E::iek_template:
    return &((E::a_template_ptr)il)->source_corresp;
  default:
    return nullptr;
  }
}

// The front end symbol of an IL entity. Symbols of closed scopes are freed
// (and possibly reused), so check that the symbol still refers to |il|.
E::a_symbol_ptr symbolOf(const void *il, E::an_il_entry_kind iek) {
  E::a_source_correspondence *scp = scpOf(il, iek);
  E::a_symbol_ptr sym = scp ? (E::a_symbol_ptr)scp->assoc_info : nullptr;
  if (!sym)
    return nullptr;
  switch (sym->kind) {
  case E::sk_class_or_struct_tag:
  case E::sk_union_tag:
    return iek == E::iek_type && sym->variant.class_struct_union.type == il ? sym : nullptr;
  case E::sk_enum_tag:
    return iek == E::iek_type && sym->variant.enumeration.type == il ? sym : nullptr;
  case E::sk_type:
    return iek == E::iek_type && sym->variant.type.ptr == il ? sym : nullptr;
  case E::sk_routine:
  case E::sk_member_function:
    return iek == E::iek_routine && sym->variant.routine.ptr == il ? sym : nullptr;
  case E::sk_variable:
    return iek == E::iek_variable && sym->variant.variable.ptr == il ? sym : nullptr;
  case E::sk_static_data_member:
    return iek == E::iek_variable && sym->variant.static_data_member.variable == il ? sym : nullptr;
  case E::sk_field:
    return iek == E::iek_field && sym->variant.field.ptr == il ? sym : nullptr;
  case E::sk_constant:
    return iek == E::iek_constant && sym->variant.constant == il ? sym : nullptr;
  case E::sk_namespace:
    return iek == E::iek_namespace && sym->variant.namespace_info.ptr == il ? sym : nullptr;
  case E::sk_class_template:
  case E::sk_function_template:
  case E::sk_variable_template:
  case E::sk_concept_template:
    return iek == E::iek_template && sym->variant.template_info &&
                   sym->variant.template_info->il_template_entry == il
               ? sym
               : nullptr;
  default:
    return nullptr;
  }
}

E::a_symbol_ptr symbolOfType(E::a_type_ptr t) { return symbolOf(t, E::iek_type); }

bool isClassType(E::a_type_ptr t) { return t && is_immediate_class_type(t); }

bool samePosition(const E::a_source_position &a, const E::a_source_position &b) {
  return a.seq == b.seq && a.column == b.column;
}

// The class of which an IL entity is a member.
E::a_type_ptr parentClass(const void *il, E::an_il_entry_kind iek) {
  E::a_source_correspondence *scp = scpOf(il, iek);
  if (!scp || !scp->is_class_member)
    return nullptr;
  if (E::a_scope_ptr ps = scp->parent_scope)
    if (ps->kind == E::sck_class_struct_union && isClassType(ps->variant.assoc_type))
      return ps->variant.assoc_type;
  return nullptr;
}

// The member of |cls| that corresponds to |il|, a member of another
// instance of the same class template: same name and declaration position.
// Uses the member lists of the IL, which (unlike the symbol table) remain
// valid at the end of the translation unit.
const void *correspondingMember(E::a_type_ptr cls, const void *il, E::an_il_entry_kind iek) {
  E::a_source_correspondence *scp = scpOf(il, iek);
  if (!scp || !scp->name || !isClassType(cls))
    return nullptr;
  E::a_class_type_supplement_ptr extra = cls->variant.class_struct_union.extra_info;
  E::a_scope_ptr scope = extra ? extra->assoc_scope : nullptr;
  const void *found = nullptr;
  int n = 0;
  bool exact = false;
  auto consider = [&](const void *m) {
    E::a_source_correspondence *mscp = scpOf(m, iek);
    if (exact || !mscp->name || strcmp(mscp->name, scp->name))
      return;
    if (samePosition(mscp->decl_position, scp->decl_position)) {
      found = m;
      exact = true;
    } else {
      found = n++ ? nullptr : m;
    }
  };
  switch (iek) {
  case E::iek_field:
    for (E::a_field_ptr f = cls->variant.class_struct_union.field_list; f; f = f->next)
      consider(f);
    break;
  case E::iek_routine:
    for (E::a_routine_ptr r = scope ? scope->routines : nullptr; r; r = r->next)
      consider(r);
    for (E::a_template_ptr t = scope ? scope->templates : nullptr; t; t = t->next)
      if (t->kind == E::templk_function && t->prototype_instantiation.routine)
        consider(t->prototype_instantiation.routine);
    break;
  case E::iek_variable:
    for (E::a_variable_ptr v = scope ? scope->variables : nullptr; v; v = v->next)
      consider(v);
    break;
  case E::iek_type:
    for (E::a_type_ptr t = scope ? scope->types : nullptr; t; t = t->next)
      consider(t);
    for (E::a_template_ptr t = scope ? scope->templates : nullptr; t; t = t->next)
      if (t->kind == E::templk_class && t->prototype_instantiation.type)
        consider(t->prototype_instantiation.type);
    break;
  default:
    break;
  }
  return found;
}

// The prototype instantiation representing the class template of the
// instance |type|. Member classes of class template instances map to the
// member classes of the prototype instantiation.
E::a_type_ptr canonicalClass(E::a_type_ptr type, int depth = 0) {
  for (; isClassType(type) && depth < 16; depth++) {
    E::a_type_ptr next = nullptr;
    E::a_class_type_supplement_ptr extra = type->variant.class_struct_union.extra_info;
    if (extra && extra->assoc_template && !type->variant.class_struct_union.is_specialized) {
      E::a_type_ptr proto = extra->assoc_template->prototype_instantiation.type;
      if (proto && proto != type)
        next = proto;
    }
    if (!next)
      if (E::a_type_ptr parent = parentClass(type, E::iek_type)) {
        E::a_type_ptr cp = canonicalClass(parent, depth + 1);
        if (cp != parent)
          next = (E::a_type_ptr)correspondingMember(cp, type, E::iek_type);
      }
    if (!next || next == type)
      break;
    type = next;
  }
  return type;
}

// A member of a class template instance -> the member of the prototype
// instantiation.
const void *canonicalMember(const void *il, E::an_il_entry_kind iek) {
  if (E::a_type_ptr parent = parentClass(il, iek)) {
    E::a_type_ptr cp = canonicalClass(parent);
    if (cp != parent)
      if (const void *m = correspondingMember(cp, il, iek))
        return m;
  }
  return il;
}

// Map an IL entity to the entity that represents it in the index: template
// instances to the prototype instantiation of their template, members of
// class template instances to the members of the prototype instantiation.
std::pair<const void *, E::an_il_entry_kind> canonicalize(const void *il, E::an_il_entry_kind iek) {
  for (int depth = 0; depth < 8; depth++) {
    switch (iek) {
    case E::iek_template: {
      E::a_template_ptr t = (E::a_template_ptr)il;
      if (t->prototype_instantiation.routine && t->kind == E::templk_function) {
        il = t->prototype_instantiation.routine, iek = E::iek_routine;
        continue;
      }
      if (t->prototype_instantiation.type && t->kind == E::templk_class)
        return {canonicalClass(t->prototype_instantiation.type), E::iek_type};
      if (t->prototype_instantiation.variable && t->kind == E::templk_variable) {
        il = t->prototype_instantiation.variable, iek = E::iek_variable;
        continue;
      }
      return {t, iek};
    }
    case E::iek_type: {
      E::a_type_ptr t = (E::a_type_ptr)il;
      if (isClassType(t))
        return {canonicalClass(t), iek};
      return {canonicalMember(t, iek), iek};
    }
    case E::iek_routine: {
      E::a_routine_ptr r = (E::a_routine_ptr)il;
      if (r->assoc_template && !r->is_specialized) {
        E::a_routine_ptr proto = r->assoc_template->prototype_instantiation.routine;
        if (proto && proto != r) {
          il = proto;
          continue;
        }
      }
      return {canonicalMember(r, iek), iek};
    }
    case E::iek_variable: {
      E::a_variable_ptr v = (E::a_variable_ptr)il;
      if (v->template_info && v->template_info->assoc_template) {
        E::a_variable_ptr proto = v->template_info->assoc_template->prototype_instantiation.variable;
        if (proto && proto != v) {
          il = proto;
          continue;
        }
      }
      return {canonicalMember(v, iek), iek};
    }
    case E::iek_field:
      return {canonicalMember(il, iek), iek};
    default:
      return {il, iek};
    }
  }
  return {il, iek};
}

int newRaw(const void *il, E::an_il_entry_kind iek) {
  int id = (int)ctx->raws.size();
  ctx->raws.push_back({il, iek, -1});
  if (il)
    ctx->il2raw[il] = id;
  return id;
}

int rawOfIL(const void *il, E::an_il_entry_kind iek) {
  if (!il)
    return -1;
  auto it = ctx->il2raw.find(il);
  if (it != ctx->il2raw.end())
    return it->second;
  return newRaw(il, iek);
}

int rawOf(E::a_symbol_ptr sym);
int localEntity(E::a_symbol_ptr sym);

// The raw entity of a type, looking through pointers, references, arrays,
// and unnamed typerefs, for e.g. the type of a variable.
int rawOfType(E::a_type_ptr t) {
  // At the end of the translation unit, symbols may be stale; types of
  // function-local entities are found via type2raw instead.
  auto viaSymbol = [](E::a_type_ptr t) {
    if (auto it = ctx->type2raw.find(t); it != ctx->type2raw.end())
      return it->second;
    if (!ctx->finalizing)
      if (E::a_symbol_ptr s = symbolOfType(t))
        return rawOf(s);
    return -1;
  };
  for (int i = 0; t && i < 32; i++) {
    switch (t->kind) {
    case E::tk_pointer:
      t = t->variant.pointer.type;
      continue;
    case E::tk_array:
      t = t->variant.array.element_type;
      continue;
    case E::tk_typeref:
      if (t->source_corresp.name) {
        if (!t->source_corresp.is_local_to_function)
          return rawOfIL(t, E::iek_type);
        if (int r = viaSymbol(t); r >= 0)
          return r;
      }
      t = t->variant.typeref.type;
      continue;
    case E::tk_class:
    case E::tk_struct:
    case E::tk_union:
      if (t->source_corresp.is_local_to_function)
        return viaSymbol(t);
      return rawOfIL(t, E::iek_type);
    case E::tk_integer:
      if (!t->variant.integer.enum_type)
        return -1;
      if (t->source_corresp.is_local_to_function)
        return viaSymbol(t);
      return rawOfIL(t, E::iek_type);
    case E::tk_template_param:
      return viaSymbol(t);
    default:
      return -1;
    }
  }
  return -1;
}

bool isLocalSCP(const E::a_source_correspondence &scp) { return scp.is_local_to_function; }

int rawOf(E::a_symbol_ptr sym) {
  if (!sym)
    return -1;
  sym = E::fundamental_symbol_of(sym);
  const void *il = nullptr;
  E::an_il_entry_kind iek = E::iek_none;
  switch (sym->kind) {
  case E::sk_class_or_struct_tag:
  case E::sk_union_tag:
    if (E::a_type_ptr t = sym->variant.class_struct_union.type; t && !isLocalSCP(t->source_corresp))
      il = t, iek = E::iek_type;
    break;
  case E::sk_enum_tag:
    if (E::a_type_ptr t = sym->variant.enumeration.type; t && !isLocalSCP(t->source_corresp))
      il = t, iek = E::iek_type;
    break;
  case E::sk_class_template:
  case E::sk_function_template:
  case E::sk_variable_template:
  case E::sk_concept_template:
    if (!sym->is_template_param && sym->variant.template_info && sym->variant.template_info->il_template_entry)
      il = sym->variant.template_info->il_template_entry, iek = E::iek_template;
    break;
  case E::sk_routine:
  case E::sk_member_function:
    if (E::a_routine_ptr r = sym->variant.routine.ptr; r && !isLocalSCP(r->source_corresp))
      il = r, iek = E::iek_routine;
    break;
  case E::sk_variable:
    if (E::a_variable_ptr v = sym->variant.variable.ptr; v && !isLocalSCP(v->source_corresp) && !v->is_parameter)
      il = v, iek = E::iek_variable;
    break;
  case E::sk_static_data_member:
    if (E::a_variable_ptr v = sym->variant.static_data_member.variable; v && !isLocalSCP(v->source_corresp))
      il = v, iek = E::iek_variable;
    break;
  case E::sk_field:
    if (E::a_field_ptr f = sym->variant.field.ptr; f && !isLocalSCP(f->source_corresp))
      il = f, iek = E::iek_field;
    break;
  case E::sk_type:
    if (!sym->is_template_param)
      if (E::a_type_ptr t = sym->variant.type.ptr; t && !isLocalSCP(t->source_corresp))
        il = t, iek = E::iek_type;
    break;
  case E::sk_constant:
    if (!sym->is_template_param)
      if (E::a_constant_ptr k = sym->variant.constant; k && !isLocalSCP(k->source_corresp))
        il = k, iek = E::iek_constant;
    break;
  case E::sk_namespace:
    if (sym->variant.namespace_info.ptr)
      il = sym->variant.namespace_info.ptr, iek = E::iek_namespace;
    break;
  case E::sk_label:
  case E::sk_keyword:
  case E::sk_undefined:
  case E::sk_overloaded_function:
  case E::sk_extern_variable:
  case E::sk_extern_routine:
    return -1;
  default:
    break;
  }
  if (il)
    return rawOfIL(il, iek);
  SymKey key{sym, (unsigned long)sym->decl_seq};
  auto it = ctx->sym2raw.find(key);
  if (it != ctx->sym2raw.end())
    return it->second;
  int id = newRaw(nullptr, E::iek_none);
  ctx->sym2raw[key] = id;
  switch (sym->kind) {
  case E::sk_type:
    ctx->type2raw[sym->variant.type.ptr] = id;
    break;
  case E::sk_class_or_struct_tag:
  case E::sk_union_tag:
    ctx->type2raw[sym->variant.class_struct_union.type] = id;
    break;
  case E::sk_enum_tag:
    ctx->type2raw[sym->variant.enumeration.type] = id;
    break;
  default:
    break;
  }
  ctx->raws[id].entity = localEntity(sym);
  return ctx->raws[id].entity < 0 ? -1 : id;
}

Storage toStorage(E::a_storage_class sc) {
  switch (sc) {
  case E::sc_extern:
    return Storage::Extern;
  case E::sc_static:
    return Storage::Static;
  case E::sc_register:
    return Storage::Register;
  default:
    return Storage::None;
  }
}

std::string identifierOf(E::a_symbol_ptr sym) {
  if (sym && sym->header && sym->header->identifier)
    return sym->header->identifier;
  return "";
}

void setName(Entity &e, std::string_view prefix, const std::string &qualified, std::string_view short_name,
             std::string_view suffix) {
  e.detailed_name.assign(prefix);
  e.qual_name_offset = (int16_t)e.detailed_name.size();
  e.detailed_name += qualified;
  size_t pos = short_name.empty() ? std::string::npos : qualified.rfind(short_name);
  if (pos == std::string::npos) {
    e.short_name_offset = (int16_t)e.detailed_name.size();
    e.short_name_size = 0;
  } else {
    e.short_name_offset = (int16_t)(e.qual_name_offset + pos);
    e.short_name_size = (int16_t)short_name.size();
  }
  e.detailed_name.append(suffix);
}

std::string fileLocation(const E::a_source_position &pos) {
  Loc loc = expansionLoc(pos);
  std::string ret = "@";
  if (loc.valid())
    ret += ctx->unit->files[loc.file].path + ":" + std::to_string(loc.line) + ":" + std::to_string(loc.column);
  return ret + "@";
}

// Whether two function types have the same parameter types, as an overrider
// and the overridden function do.
bool sameParameters(E::a_type_ptr a, E::a_type_ptr b) {
  E::a_routine_type_supplement_ptr ia = routineTypeInfo(a), ib = routineTypeInfo(b);
  if (!ia || !ib || ia->has_ellipsis != ib->has_ellipsis)
    return false;
  E::a_param_type_ptr pa = ia->param_type_list, pb = ib->param_type_list;
  for (; pa && pb; pa = pa->next, pb = pb->next)
    if (!identical_types(pa->type, pb->type))
      return false;
  return !pa && !pb;
}

// The name of an IL entity as written, or "" for an unnamed entity (which may
// have been given a name for mangling).
const char *writtenName(const E::a_source_correspondence *scp) {
#if NEED_NAME_MANGLING
  if (scp->unnamed_entity_given_fabricated_name)
    return "";
#endif
  return scp->name ? scp->name : "";
}

bool inUnnamedNamespace(E::a_source_correspondence *scp) {
  for (E::a_scope_ptr ps = scp->parent_scope; ps; ps = ps->parent)
    if (ps->kind == E::sck_namespace && ps->variant.assoc_namespace &&
        !*writtenName(&ps->variant.assoc_namespace->source_corresp))
      return true;
  return false;
}

int newEntity() {
  ctx->unit->entities.emplace_back();
  return (int)ctx->unit->entities.size() - 1;
}

// The raw entity of the routine whose body is being parsed.
int currentFunctionRaw() {
  if (E::innermost_function_scope)
    if (E::a_routine_ptr r = E::innermost_function_scope->variant.routine.ptr) {
      // A lambda or a member function of a local class. Its IL lives in the
      // memory of the enclosing function, so describe it via its symbol.
      if (r->source_corresp.is_local_to_function) {
        if (E::a_symbol_ptr sym = symbolOf(r, E::iek_routine))
          return rawOf(sym);
        return -1;
      }
      return rawOfIL(r, E::iek_routine);
    }
  return -1;
}

void describeVar(Entity &e, E::a_type_ptr type, const std::string &qualified, const std::string &short_name,
                 bool is_static, E::a_variable_ptr v = nullptr, std::string_view suffix1 = "") {
  if (type) {
    // Show the deduced type of "auto x = ...".
    E::a_type_ptr shown = type;
    while (shown->kind == E::tk_typeref && shown->variant.typeref.type &&
           (shown->variant.typeref.kind == E::trk_is_deduced_auto ||
            shown->variant.typeref.kind == E::trk_is_deduced_decltype_auto ||
            shown->variant.typeref.kind == E::trk_is_deduced_class))
      shown = shown->variant.typeref.type;
    auto [prefix, suffix] = declaratorParts(shown);
    // Closure types print as "lambda [] type at line N, col. M".
    if (isClassType(shown) && shown->variant.class_struct_union.extra_info &&
        shown->variant.class_struct_union.extra_info->is_lambda_closure_class)
      prefix = "(lambda) ";
    if (v && v->is_constexpr) {
      if (prefix.rfind("const ", 0) == 0)
        prefix.erase(0, 6);
      prefix = "constexpr " + prefix;
    }
    if (v && v->is_inline && !v->is_constexpr)
      prefix = "inline " + prefix;
    if (v && v->is_thread_local)
      prefix = "thread_local " + prefix;
    if (is_static)
      prefix = "static " + prefix;
    else if (e.storage == Storage::Extern)
      prefix = "extern " + prefix;
    setName(e, prefix, qualified, short_name, suffix + std::string(suffix1));
    e.type = rawOfType(type);
  } else {
    setName(e, "", qualified, short_name, "");
  }
}

// Describe a function-local entity (local variable, parameter, local class,
// template parameter) or a macro.
int localEntity(E::a_symbol_ptr sym) {
  std::string name = identifierOf(sym);
  int id = newEntity();
  Entity e;
  e.short_name = name;
  e.is_local = true;
  std::string usr = "L:" + name + fileLocation(sym->decl_position);
  switch (sym->kind) {
  case E::sk_macro:
    e.kind = EK::Var, e.symbol_kind = SK_Macro;
    e.is_local = false;
    usr = "@macro@" + name;
    // Different macros with the same name are distinct, except in system
    // headers, which may define a macro in several configurations.
    if (Loc loc = expansionLoc(sym->decl_position); loc.valid() && !ctx->unit->files[loc.file].system)
      usr += fileLocation(sym->decl_position);
    setName(e, "", name, name, "");
    break;
  case E::sk_type:
  case E::sk_class_template:
    e.kind = EK::Type;
    e.symbol_kind = sym->is_template_param ? SK_TypeParameter : SK_TypeAlias;
    setName(e, "", name, name, "");
    break;
  case E::sk_class_or_struct_tag:
  case E::sk_union_tag:
  case E::sk_enum_tag: {
    e.kind = EK::Type;
    e.symbol_kind = sym->kind == E::sk_enum_tag ? SK_Enum : SK_Class;
    if (sym->kind == E::sk_class_or_struct_tag)
      if (E::a_type_ptr t = sym->variant.class_struct_union.type; t && t->kind == E::tk_struct)
        e.symbol_kind = SK_Struct;
    std::string q = symbolName(sym);
    setName(e, sym->kind == E::sk_enum_tag ? "enum " : sym->kind == E::sk_union_tag ? "union " : "struct ", q,
            name, "");
    break;
  }
  case E::sk_variable: {
    e.kind = EK::Var;
    E::a_variable_ptr v = sym->variant.variable.ptr;
    e.symbol_kind = v && v->is_parameter ? SK_Parameter : SK_Variable;
    if (v)
      e.storage = toStorage(v->storage_class);
    describeVar(e, v ? v->type : nullptr, name, name, e.storage == Storage::Static, v);
    break;
  }
  case E::sk_parameter:
    e.kind = EK::Var, e.symbol_kind = SK_Parameter;
    describeVar(e, sym->variant.param_id ? sym->variant.param_id->type : nullptr, name, name, false);
    break;
  case E::sk_static_data_member:
  case E::sk_field: {
    e.kind = EK::Var, e.symbol_kind = SK_Field;
    E::a_type_ptr type = nullptr;
    if (sym->kind == E::sk_field) {
      if (sym->variant.field.ptr)
        type = sym->variant.field.ptr->type;
    } else if (sym->variant.static_data_member.variable) {
      type = sym->variant.static_data_member.variable->type;
    }
    describeVar(e, type, symbolName(sym), name, false);
    break;
  }
  case E::sk_constant: {
    e.kind = EK::Var;
    e.symbol_kind = sym->is_template_param ? SK_Parameter : SK_EnumMember;
    E::a_type_ptr type = sym->variant.constant ? sym->variant.constant->type : nullptr;
    describeVar(e, sym->is_template_param ? type : nullptr, name, name, false);
    break;
  }
  case E::sk_routine:
  case E::sk_member_function:
    // Block scope function declaration.
    e.kind = EK::Func, e.symbol_kind = SK_Function;
    setName(e, "", name, name, "()");
    break;
  default:
    ctx->unit->entities.pop_back();
    return -1;
  }
  e.usr = std::move(usr);
  ctx->unit->entities[id] = std::move(e);
  return id;
}

const char *classTag(E::a_type_ptr t) {
  switch (t->kind) {
  case E::tk_struct:
    return "struct";
  case E::tk_union:
    return "union";
  default:
    return "class";
  }
}

// Describe a persistent IL entity.
int ilEntity(const void *il, E::an_il_entry_kind iek) {
  // Front end symbols may be stale at the end of the translation unit; use the
  // IL only.
  E::a_source_correspondence *scp = scpOf(il, iek);
  std::string short_name = writtenName(scp);
  std::string qualified = ilName(scp, iek);
  for (size_t i; (i = qualified.find("<unnamed>")) != std::string::npos;)
    qualified.replace(i, 9, scp && inUnnamedNamespace(scp) ? "(anon ns)" : "(anon)");
  int id = newEntity();
  ctx->canon2entity[il] = id;
  Entity e;
  e.short_name = short_name;
  if (scp->parent_scope) {
    E::a_scope_ptr ps = scp->parent_scope;
    if (ps->kind == E::sck_class_struct_union && ps->variant.assoc_type)
      e.parent = rawOfIL(ps->variant.assoc_type, E::iek_type);
    else if (ps->kind == E::sck_namespace && ps->variant.assoc_namespace)
      e.parent = rawOfIL(ps->variant.assoc_namespace, E::iek_namespace);
  }
  bool internal = scp && scp->name_linkage == E::nlk_internal;
  std::string usr;

  switch (iek) {
  case E::iek_namespace: {
    e.kind = EK::Type, e.symbol_kind = SK_Namespace;
    if (short_name.empty()) {
      e.detailed_name = "anon ns";
      e.short_name_size = (int16_t)e.detailed_name.size();
      usr = "N:" + qualified + fileLocation(scp->decl_position);
    } else {
      setName(e, "namespace ", qualified, short_name, " {}");
      usr = "N:" + qualified;
    }
    E::a_namespace_ptr ns = (E::a_namespace_ptr)il;
    if (ns->is_namespace_alias) {
      e.alias_of = rawOfIL(ns->variant.assoc_namespace, E::iek_namespace);
      if (e.alias_of >= 0)
        e.detailed_name = "namespace " + e.detailed_name;
    } else if (e.parent >= 0) {
      e.bases.push_back(e.parent);
    }
    break;
  }
  case E::iek_type: {
    E::a_type_ptr t = (E::a_type_ptr)il;
    e.kind = EK::Type;
    if (isClassType(t)) {
      e.symbol_kind = t->kind == E::tk_struct ? SK_Struct : SK_Class;
      std::string tag = classTag(t);
      std::string display = qualified;
      // The primary template is named without template arguments.
      if (t->variant.class_struct_union.is_prototype_instantiation && display.size() && display.back() == '>' &&
          !t->variant.class_struct_union.is_specialized) {
        int depth = 0;
        for (size_t i = display.size(); i-- > 0;) {
          if (display[i] == '>')
            depth++;
          else if (display[i] == '<' && --depth == 0) {
            if (display.compare(i - std::min(i, short_name.size()), short_name.size(), short_name) == 0)
              display.resize(i);
            break;
          }
        }
      }
      E::a_class_type_supplement_ptr extra = t->variant.class_struct_union.extra_info;
      std::string bases;
      if (extra)
        for (E::a_base_class_ptr b = extra->base_classes; b; b = b->next)
          if (b->direct)
            bases += (bases.empty() ? " : " : ", ") + typeToString(b->type);
      if (short_name.empty()) {
        e.detailed_name = "anon " + tag;
        e.short_name_size = (int16_t)e.detailed_name.size();
        usr = "T:" + qualified + fileLocation(scp->decl_position);
      } else {
        setName(e, tag + " ", display, short_name, E::is_incomplete_type(t) ? "" : bases + " {}");
        usr = "T:" + qualified;
      }
      if (extra)
        for (E::a_base_class_ptr b = extra->base_classes; b; b = b->next)
          if (b->direct) {
            int r = rawOfType(b->type);
            if (r >= 0)
              e.bases.push_back(r);
          }
      // A specialization is "derived" from the primary template.
      E::a_type_ptr primary = nullptr;
      if (t->variant.class_struct_union.is_specialized && extra && extra->assoc_template)
        primary = canonicalClass(extra->assoc_template->prototype_instantiation.type);
      if (primary && primary != t)
        e.bases.push_back(rawOfIL(primary, E::iek_type));
      for (E::a_field_ptr f = t->variant.class_struct_union.field_list; f; f = f->next) {
        if (!f->source_corresp.name)
          continue;
        int64_t offset = -1;
        if (!t->variant.class_struct_union.is_prototype_instantiation)
          offset = (int64_t)f->offset * 8 + (int64_t)f->offset_bit_remainder;
        e.fields.emplace_back(rawOfIL(f, E::iek_field), offset);
      }
    } else if (t->kind == E::tk_integer && t->variant.integer.enum_type) {
      e.symbol_kind = SK_Enum;
      if (short_name.empty()) {
        e.detailed_name = "anon enum";
        e.short_name_size = (int16_t)e.detailed_name.size();
        usr = "T:" + qualified + fileLocation(scp->decl_position);
      } else {
        std::string base;
        if (E::an_integer_type_supplement_ptr ix = t->variant.integer.extra_info;
            ix && t->variant.integer.has_explicit_enum_base && ix->base_type)
          base = " : " + typeToString(ix->base_type);
        setName(e, t->variant.integer.is_scoped_enum ? "enum class " : "enum ", qualified, short_name, base + " {}");
        usr = "T:" + qualified;
      }
    } else if (t->kind == E::tk_typeref) {
      e.symbol_kind = SK_TypeAlias;
      std::string under = typeToString(t->variant.typeref.type);
      if (t->variant.typeref.kind == E::trk_is_alias)
        setName(e, "using ", qualified, short_name, " = " + under);
      else
        setName(e, "typedef " + under + (under.size() ? " " : ""), qualified, short_name, "");
      usr = "T:" + qualified;
      e.alias_of = rawOfType(t->variant.typeref.type);
    } else {
      e.symbol_kind = SK_TypeAlias;
      setName(e, "", qualified, short_name, "");
      usr = "T:" + qualified;
    }
    break;
  }
  case E::iek_routine: {
    E::a_routine_ptr r = (E::a_routine_ptr)il;
    e.kind = EK::Func;
    E::a_routine_type_supplement_ptr tinfo = routineTypeInfo(r->type);
    bool is_static_member = scp->is_class_member && r->special_kind == E::sfk_none && tinfo && !tinfo->this_class;
    if (scp->is_class_member) {
      if (r->special_kind == E::sfk_constructor)
        e.symbol_kind = SK_Constructor;
      else if (is_static_member)
        e.symbol_kind = SK_StaticMethod;
      else
        e.symbol_kind = SK_Method;
    } else {
      e.symbol_kind = SK_Function;
    }
    // Implicit external linkage is not a storage class.
    if (r->storage_class == E::sc_static && (is_static_member || !inUnnamedNamespace(scp)))
      e.storage = Storage::Static;
    std::pair<std::string, std::string> parts{"", "()"};
    if (r->type)
      parts = routineParts(r->type);
    auto &[prefix, suffix] = parts;
    if (r->special_kind == E::sfk_constructor || r->special_kind == E::sfk_destructor ||
        r->special_kind == E::sfk_conversion)
      prefix.clear();
    if (r->is_declared_constexpr)
      prefix = (r->is_consteval ? "consteval " : "constexpr ") + prefix;
    // An overrider is virtual implicitly.
    if (r->is_virtual && !r->overrides_base_member)
      prefix = "virtual " + prefix;
    else if (e.storage == Storage::Static || is_static_member)
      prefix = "static " + prefix;
    if (r->override)
      suffix += " override";
    if (r->final)
      suffix += " final";
    if (r->pure_virtual)
      suffix += " = 0";
    setName(e, prefix, qualified, short_name, suffix);
    usr = "F:" + qualified + typeToString(r->type);
    if (internal)
      usr = fileLocation(scp->decl_position) + usr;
    if (r->overrides_base_member && scp->name)
      if (E::a_type_ptr cls = parentClass(r, E::iek_routine)) {
        E::a_class_type_supplement_ptr extra = cls->variant.class_struct_union.extra_info;
        for (E::a_base_class_ptr b = extra ? extra->base_classes : nullptr; b; b = b->next) {
          E::a_type_ptr bt = b->type;
          E::a_class_type_supplement_ptr bextra = isClassType(bt) ? bt->variant.class_struct_union.extra_info : nullptr;
          E::a_scope_ptr bscope = bextra ? bextra->assoc_scope : nullptr;
          for (E::a_routine_ptr br = bscope ? bscope->routines : nullptr; br; br = br->next) {
            if (!br->is_virtual || br->special_kind != r->special_kind)
              continue;
            if (r->special_kind != E::sfk_destructor &&
                (!br->source_corresp.name || strcmp(br->source_corresp.name, scp->name) ||
                 !sameParameters(br->type, r->type)))
              continue;
            e.bases.push_back(rawOfIL(br, E::iek_routine));
          }
        }
      }
    break;
  }
  case E::iek_variable: {
    E::a_variable_ptr v = (E::a_variable_ptr)il;
    e.kind = EK::Var;
    e.symbol_kind = scp->is_class_member ? SK_Field : SK_Variable;
    e.storage = toStorage(v->storage_class);
    if (e.storage == Storage::Static && !scp->is_class_member && inUnnamedNamespace(scp))
      e.storage = Storage::None;
    describeVar(e, v->type, qualified, short_name, e.storage == Storage::Static || scp->is_class_member, v);
    usr = "V:" + qualified;
    if (internal)
      usr = fileLocation(scp->decl_position) + usr;
    break;
  }
  case E::iek_field: {
    E::a_field_ptr f = (E::a_field_ptr)il;
    e.kind = EK::Var, e.symbol_kind = SK_Field;
    describeVar(e, f->type, qualified, short_name, false, nullptr,
                f->is_bit_field ? " : " + std::to_string(f->bit_size) : std::string());
    usr = "V:" + qualified;
    if (short_name.empty())
      usr += fileLocation(scp->decl_position);
    break;
  }
  case E::iek_constant: {
    E::a_constant_ptr k = (E::a_constant_ptr)il;
    e.kind = EK::Var, e.symbol_kind = SK_EnumMember;
    setName(e, "", qualified, short_name, "");
    usr = "V:" + qualified;
    if (k->type && k->type->kind == E::tk_integer && k->type->variant.integer.enum_type) {
      e.parent = e.type = rawOfIL(k->type, E::iek_type);
      if (k->kind == E::ck_integer) {
        ctx->out.clear();
        E::form_integer_constant(k, TRUE, FALSE, &ctx->octl);
        std::string value = " = " + takeOut();
        // Show the value in the name if it is written.
        if (scp->decl_pos_info && scp->decl_pos_info->variant.enum_value_range.start.seq)
          e.detailed_name += value;
        else
          e.hover = e.detailed_name + value;
      }
    }
    break;
  }
  case E::iek_template: {
    E::a_template_ptr t = (E::a_template_ptr)il;
    e.kind = EK::Type;
    e.symbol_kind = t->kind == E::templk_concept ? SK_Interface : SK_TypeAlias;
    setName(e, t->kind == E::templk_concept ? "concept " : "", qualified, short_name, "");
    usr = "T:" + qualified;
    break;
  }
  default:
    e.kind = EK::Invalid;
    break;
  }
  e.usr = std::move(usr);
  ctx->unit->entities[id] = std::move(e);
  return id;
}

int resolve(int raw) {
  if (raw < 0)
    return -1;
  if (ctx->raws[raw].entity >= 0 || !ctx->raws[raw].il)
    return ctx->raws[raw].entity;
  auto [il, iek] = canonicalize(ctx->raws[raw].il, ctx->raws[raw].iek);
  auto it = ctx->canon2entity.find(il);
  int id = it != ctx->canon2entity.end() ? it->second : ilEntity(il, iek);
  ctx->raws[raw].entity = id;
  return id;
}

uint16_t toRole(E::a_symbol_reference_kind srk) {
  uint16_t role = 0;
  if (srk & SRK_DEFINITION)
    role |= RoleDefinition;
  else if (srk & SRK_DECLARATION)
    role |= RoleDeclaration;
  else
    role |= RoleReference;
  if (srk & (SRK_USE | SRK_CONST_VALUE_USE))
    role |= RoleRead;
  if (srk & SRK_MODIFICATION)
    role |= RoleWrite;
  if (srk & SRK_ADDRESS_TAKEN)
    role |= RoleAddress;
  if (srk & SRK_IMPLICIT)
    role |= RoleImplicit;
  return role;
}

bool wantFile(int file) {
  if (!ctx->req->want_file)
    return true;
  if ((size_t)file >= ctx->wanted.size())
    ctx->wanted.resize(file + 1);
  if (!ctx->wanted[file])
    ctx->wanted[file] = ctx->req->want_file(file, ctx->unit->files[file].path) ? 1 : 2;
  return ctx->wanted[file] == 1;
}

void onReference(E::a_symbol_reference_kind srk, E::a_symbol_ptr sym, E::a_source_position *pos) {
  if (!ctx || !ctx->req->index)
    return;
  if (srk & (SRK_ERROR | SRK_TEMPLATE_INSTANTIATION))
    return;
  Loc loc = spellingLoc(*pos);
  if (!loc.valid() || !wantFile(loc.file))
    return;
  int raw = rawOf(sym);
  if (raw < 0)
    return;
  Occurrence occ;
  occ.entity = raw;
  occ.role = toRole(srk);
  occ.loc = loc;
  E::a_symbol_ptr fsym = E::fundamental_symbol_of(sym);
  if (occ.role & (RoleDeclaration | RoleDefinition)) {
    if (fsym->kind == E::sk_routine || fsym->kind == E::sk_member_function ||
        fsym->kind == E::sk_function_template) {
      ctx->last_routine = raw;
      for (int p : ctx->orphan_params)
        if (int e = ctx->raws[p].entity; e >= 0 && ctx->unit->entities[e].parent < 0)
          ctx->unit->entities[e].parent = raw;
      ctx->orphan_params.clear();
    } else if (fsym->kind == E::sk_parameter) {
      // Parameters of a function declaration (not definition) have no
      // enclosing function scope. The routine is declared before or after
      // its parameters.
      occ.role = RoleDefinition;
      if (int e = ctx->raws[raw].entity; e >= 0 && ctx->unit->entities[e].parent < 0) {
        Occurrence *last = ctx->unit->occurrences.size() ? &ctx->unit->occurrences.back() : nullptr;
        if (last && last->entity == ctx->last_routine && last->loc.file == loc.file && last->loc.line == loc.line)
          ctx->unit->entities[e].parent = ctx->last_routine;
        else
          ctx->orphan_params.push_back(raw);
      }
    }
  }
  if (!(occ.role & (RoleDeclaration | RoleDefinition))) {
    occ.container = currentFunctionRaw();
  } else {
    if (int e = ctx->raws[raw].entity; e >= 0) {
      // A local entity's semantic parent is the function it is declared in.
      Entity &ent = ctx->unit->entities[e];
      if (ent.is_local && ent.parent < 0)
        ent.parent = currentFunctionRaw();
    }
    PendingExtent pe{ctx->unit->occurrences.size(), ctx->raws[raw].il, ctx->raws[raw].iek, nullptr};
    if (!pe.il) {
      E::a_symbol_ptr fs = E::fundamental_symbol_of(sym);
      if (fs->kind == E::sk_variable && fs->variant.variable.ptr && E::innermost_function_scope) {
        pe.il = fs->variant.variable.ptr;
        pe.iek = E::iek_variable;
        pe.routine = E::innermost_function_scope->variant.routine.ptr;
      }
    }
    if (pe.il)
      ctx->extents.push_back(pe);
  }
  ctx->unit->occurrences.push_back(occ);
}

bool before(const E::a_source_position &a, const E::a_source_position &b) {
  return a.seq < b.seq || (a.seq == b.seq && a.column < b.column);
}

// The scope of the body of |r| if its IL is still in memory. The bodies of
// lambdas and local classes' member functions are in the memory region of the
// enclosing function.
E::a_scope_ptr functionScope(E::a_routine_ptr r) {
  for (int i = 0; i < 16 && r->source_corresp.is_local_to_function; i++) {
    E::a_routine_ptr outer = r->source_corresp.enclosing_routine;
    // A member function of a closure or local class: the class is local.
    if (!outer)
      if (E::a_scope_ptr ps = r->source_corresp.parent_scope)
        if (ps->kind == E::sck_class_struct_union && ps->variant.assoc_type)
          outer = ps->variant.assoc_type->source_corresp.enclosing_routine;
    if (!outer)
      break;
    r = outer;
  }
  E::a_memory_region_number region = r->memory_region;
  if (region <= 0 || region > E::highest_used_region_number || !E::il_header.region_scope_entry)
    return nullptr;
  E::a_scope_ptr scope = E::il_header.region_scope_entry[region];
  if (!scope || scope->kind != E::sck_function || scope->variant.routine.ptr != r)
    return nullptr;
  return scope;
}

void computeExtent(const PendingExtent &pe) {
  Occurrence &occ = ctx->unit->occurrences[pe.occ];
  if (pe.routine && !functionScope(pe.routine))
    return;
  if (pe.routine && pe.iek == E::iek_variable) {
    // The type of "auto x = ..." is deduced after the declaration is seen.
    E::a_variable_ptr v = (E::a_variable_ptr)pe.il;
    if (int e = ctx->raws[occ.entity].entity; e >= 0 && v->type) {
      Entity &ent = ctx->unit->entities[e];
      std::string name = ent.short_name;
      describeVar(ent, v->type, name, name, ent.storage == Storage::Static, v);
    }
  }
  E::a_source_correspondence *scp = scpOf(pe.il, pe.iek);
  if (!scp || !scp->decl_pos_info)
    return;
  E::a_decl_position_supplement_ptr d = scp->decl_pos_info;
  // The supplement describes one declaration; skip the others.
  const E::a_source_position &id = d->identifier_range.start.seq ? d->identifier_range.start : scp->decl_position;
  // The identifier may be qualified, e.g. "Foo::member".
  Loc idl = spellingLoc(id);
  int id_end = d->identifier_range.end.seq ? spellingLoc(d->identifier_range.end).column : idl.column;
  if (idl.file != occ.loc.file || idl.line != occ.loc.line || occ.loc.column < idl.column ||
      occ.loc.column > std::max(id_end, idl.column))
    return;
  const E::a_source_position *b = nullptr, *e = nullptr;
  auto add = [&](const E::a_source_range &r) {
    if (r.start.seq == 0 || r.end.seq == 0)
      return;
    if (!b || before(r.start, *b))
      b = &r.start;
    if (!e || before(*e, r.end))
      e = &r.end;
  };
  add(d->specifiers_range);
  add(d->identifier_range);
  switch (pe.iek) {
  case E::iek_constant:
    add(d->variant.enum_value_range);
    break;
  case E::iek_namespace:
    add(d->variant.namespace_definition_range);
    break;
  default:
    add(d->variant.declarator_range);
    break;
  }
  const E::a_source_range *init = nullptr;
  if (pe.iek == E::iek_variable)
    init = &((E::a_variable_ptr)pe.il)->initializer_range;
  else if (pe.iek == E::iek_field)
    init = &((E::a_field_ptr)pe.il)->initializer_range;
  if (init && init->start.seq && init->end.seq) {
    add(*init);
    Loc ib = expansionLoc(init->start), ie = expansionLoc(init->end);
    if (ib.file == occ.loc.file && ie.file == occ.loc.file) {
      occ.init_begin = ib;
      occ.init_end = ie;
    }
  }
  if (!b)
    return;
  E::a_source_position body_end{};
  if (pe.iek == E::iek_routine && (occ.role & RoleDefinition))
    if (E::a_scope_ptr scope = functionScope((E::a_routine_ptr)pe.il))
      if (scope->assoc_block && scope->assoc_block->end_position.seq)
        body_end = scope->assoc_block->end_position;
  if (body_end.seq && before(*e, body_end))
    e = &body_end;
  Loc lb = expansionLoc(*b), le = expansionLoc(*e);
  if (lb.file == occ.loc.file && le.file == occ.loc.file) {
    occ.extent_begin = lb;
    occ.extent_end = le;
  }
}

void finalize() {
  Unit &u = *ctx->unit;
  ctx->finalizing = true;
  std::vector<E::a_source_file_ptr> stack;
  for (E::a_source_file_ptr sf = E::il_header.primary_source_file; sf; sf = sf->next)
    stack.push_back(sf);
  while (stack.size()) {
    E::a_source_file_ptr sf = stack.back();
    stack.pop_back();
    int parent = fileId(sf);
    for (E::a_source_file_ptr child = sf->first_child_file; child; child = child->next) {
      int cid = fileId(child);
      E::a_line_number line = 0;
      E::a_boolean at_end;
      if (child->first_seq_number > 1)
        E::source_file_for_seq(child->first_seq_number - 1, &line, &at_end, TRUE);
      u.includes.push_back({parent, (int)line, cid});
      stack.push_back(child);
    }
  }

  for (const PendingExtent &pe : ctx->extents)
    computeExtent(pe);

  // Describing an entity may reference more raw entities.
  for (size_t i = 0; i < ctx->raws.size(); i++)
    resolve((int)i);
  auto map = [&](int &raw) {
    if (raw >= 0)
      raw = ctx->raws[raw].entity;
  };
  for (Entity &e : u.entities) {
    map(e.parent);
    map(e.type);
    map(e.alias_of);
    for (int &b : e.bases)
      map(b);
    for (auto &f : e.fields)
      map(f.first);
  }
  for (size_t i = 0; i < u.entities.size(); i++) {
    Entity &e = u.entities[i];
    e.parent_kind = e.parent >= 0 ? u.entities[e.parent].symbol_kind : SK_File;
    e.bases.erase(std::remove_if(e.bases.begin(), e.bases.end(), [&](int b) { return b < 0 || b == (int)i; }),
                  e.bases.end());
  }
  for (Occurrence &o : u.occurrences) {
    map(o.entity);
    map(o.container);
  }
  u.occurrences.erase(std::remove_if(u.occurrences.begin(), u.occurrences.end(),
                                     [&](const Occurrence &o) {
                                       return o.entity < 0 || u.entities[o.entity].kind == EK::Invalid;
                                     }),
                      u.occurrences.end());
}

void onSkippedRange(E::a_source_position *start, E::a_source_position *end) {
  if (!ctx || !ctx->req->index)
    return;
  Loc b = expansionLoc(*start), e = expansionLoc(*end);
  if (b.valid() && e.valid() && b.file == e.file && wantFile(b.file))
    ctx->unit->skipped.push_back({b, e});
}

FILE *openHook(E::a_const_char *name) {
  if (!ctx || ctx->remapped.empty())
    return nullptr;
  auto it = ctx->remapped.find(name);
  if (it == ctx->remapped.end()) {
    char buf[PATH_MAX];
    if (!realpath(name, buf))
      return nullptr;
    it = ctx->remapped.find(buf);
    if (it == ctx->remapped.end())
      return nullptr;
  }
  const std::string &content = *it->second;
  if (content.empty())
    return fopen("/dev/null", "r");
  return fmemopen((void *)content.data(), content.size(), "r");
}
} // namespace

void setBaseDirectory(const std::string &dir) { g_base_dir = dir; }

void recover() {
  ctx = nullptr;
  E::symbol_reference_callback = nullptr;
  E::source_file_open_hook = nullptr;
  E::skipped_range_callback = nullptr;
  // An error during cleanup longjmps to edg_main_setjmp_buffer, which must be
  // armed in a live frame.
  if (setjmp(E::edg_main_setjmp_buffer) == 0)
    E::fe_cleanup();
}

const char *version() { return VERSION_NUMBER; }

bool run(const Request &req, Unit &unit) {
  Ctx c;
  c.req = &req;
  c.unit = &unit;
  for (auto &[path, content] : req.remapped)
    c.remapped[path] = &content;
  ctx = &c;
  initOctl();

  std::vector<std::string> args{"edgls"};
  if (g_base_dir.size()) {
    args.push_back("--edg_base_dir");
    args.push_back(g_base_dir);
  }
  args.push_back("--output_mode");
  args.push_back("sarif");
  args.insert(args.end(), req.args.begin(), req.args.end());
  std::vector<char *> argv;
  for (auto &a : args)
    argv.push_back(a.data());
  argv.push_back(nullptr);

  E::symbol_reference_callback = req.index ? onReference : nullptr;
  E::source_file_open_hook = openHook;
  E::skipped_range_callback = req.index ? onSkippedRange : nullptr;
  E::edg_main((int)args.size(), argv.data());
  E::symbol_reference_callback = nullptr;
  E::source_file_open_hook = nullptr;
  E::skipped_range_callback = nullptr;
  ctx = nullptr;

  for (auto &ms : c.streams) {
    if (E::f_error == ms->file) {
      fclose(ms->file);
      E::f_error = nullptr;
    }
    if (E::f_pp_output == ms->file) {
      fclose(ms->file);
      E::f_pp_output = nullptr;
    }
  }
  for (auto &ms : c.streams) {
    if (ms->buf)
      unit.sarif.append(ms->buf, ms->size);
    free(ms->buf);
  }
  return unit.completed;
}

namespace detail {
void backEnd() {
  if (!ctx)
    return;
  if (ctx->req->index)
    finalize();
  ctx->unit->n_errors = (int)E::diagnostic_counters.total.errors;
  ctx->unit->completed = true;
}

FILE *newOutputFile() {
  if (!ctx)
    return stderr;
  auto ms = std::make_unique<MemStream>();
  ms->file = open_memstream(&ms->buf, &ms->size);
  FILE *ret = ms->file;
  ctx->streams.push_back(std::move(ms));
  return ret;
}
} // namespace detail
} // namespace ccls::edg

// Hooks required by the front end configuration.
void back_end(void) { ccls::edg::detail::backEnd(); }

namespace edg {
FILE *default_error_output_file() { return ccls::edg::detail::newOutputFile(); }
FILE *default_preproc_output_file() { return ccls::edg::detail::newOutputFile(); }
} // namespace edg
