// Copyright 2026 edgls Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "lsp.hh"

#include <string>
#include <vector>

namespace ccls {
struct EdgInvocation {
  // Front end options, ending with the main file.
  std::vector<std::string> args;
  LanguageId lang = LanguageId::Unknown;
};

// Translate a GCC/Clang command line (args[0] is the compiler) compiling
// |main| to front end options. Returns false if the file is not C/C++
// (e.g. assembly) or the command line has no compiler.
bool buildEdgInvocation(const std::vector<const char *> &args, const std::string &main, EdgInvocation &inv);
} // namespace ccls
