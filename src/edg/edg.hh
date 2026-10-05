// Copyright 2026 edgls Authors
// SPDX-License-Identifier: Apache-2.0

// Interface to the EDG C/C++ front end. Only src/edg/*.cc include EDG headers,
// whose macros (TRUE, DEBUG, CHECKING, ...) would collide with the rest of
// the code base.

#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace ccls::edg {
// A source location. line and column are 1-based; column counts bytes.
struct Loc {
  int file = -1;
  int line = 0;
  int column = 0;
  bool valid() const { return file >= 0 && line > 0; }
};

// Mirrors ccls::Kind.
enum class EntityKind : uint8_t { Invalid, File, Type, Func, Var };

// Mirrors ccls::Role.
enum RoleBits : uint16_t {
  RoleDeclaration = 1 << 0,
  RoleDefinition = 1 << 1,
  RoleReference = 1 << 2,
  RoleRead = 1 << 3,
  RoleWrite = 1 << 4,
  RoleCall = 1 << 5,
  RoleDynamic = 1 << 6,
  RoleAddress = 1 << 7,
  RoleImplicit = 1 << 8,
};

// Mirrors ccls::StorageClass.
enum class Storage : uint8_t { None, Extern, Static, PrivateExtern, Auto, Register };

struct Entity {
  std::string usr;
  EntityKind kind = EntityKind::Invalid;
  uint8_t symbol_kind = 0; // LSP SymbolKind, including ccls extensions
  uint8_t parent_kind = 0;
  Storage storage = Storage::None;
  bool is_local = false;

  std::string short_name;
  std::string detailed_name;
  int16_t qual_name_offset = 0;
  int16_t short_name_offset = 0;
  int16_t short_name_size = 0;
  std::string hover;

  // Semantic parent: the enclosing class, namespace, or function.
  int parent = -1;
  // Type: base classes, or the enclosing namespace of a namespace.
  // Func: overridden methods.
  std::vector<int> bases;
  // Var: the type (entity index), or 0 for builtin/unknown types.
  int type = -1;
  // Type: the type an alias refers to.
  int alias_of = -1;
  // Type: data members with their offsets in bits (-1 if unknown).
  std::vector<std::pair<int, int64_t>> fields;
};

struct Occurrence {
  int entity;
  uint16_t role;
  Loc loc;
  // End of the name, if known (exclusive). Otherwise one token is assumed.
  Loc loc_end;
  // Declarations and definitions: the extent of the declaration (inclusive
  // end position of the last token).
  Loc extent_begin, extent_end;
  // Variables: the initializer, including "=" if present.
  Loc init_begin, init_end;
  // The function whose body contains this reference, or -1.
  int container = -1;
};

struct Include {
  int file;
  int line;
  int included_file;
};

struct File {
  std::string path;
  bool system = false;
};

struct SkippedRange {
  // The line of the directive starting the region and the "#" of the
  // directive ending it.
  Loc begin, end;
};

struct Unit {
  std::vector<File> files;
  std::vector<Include> includes;
  // Regions skipped by false preprocessor conditionals.
  std::vector<SkippedRange> skipped;
  std::vector<Entity> entities;
  std::vector<Occurrence> occurrences;
  // Diagnostics in the SARIF format.
  std::string sarif;
  int n_errors = 0;
  // Whether the front end ran to completion (possibly with errors).
  bool completed = false;
};

struct Request {
  // Front end command line arguments, excluding argv[0].
  std::vector<std::string> args;
  // Files whose contents override those on disk.
  std::vector<std::pair<std::string, std::string>> remapped;
  // Collect entities and occurrences.
  bool index = true;
  // Also index entities without linkage (locals, parameters).
  bool all_linkages = true;
  // If set, called once for each file; occurrences in files for which it
  // returns false are not collected.
  std::function<bool(int file, const std::string &path)> want_file;
};

// Run the front end. Can be called concurrently from different threads.
bool run(const Request &req, Unit &unit);

// Release the front end's state after a crash in run().
void recover();

// Directory containing lib/predefined_macros.txt.
void setBaseDirectory(const std::string &dir);

// The front end version, e.g. "7.0".
const char *version();
} // namespace ccls::edg
