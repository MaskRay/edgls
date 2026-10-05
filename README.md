# edgls

edgls is a C/C++ language server built on the [EDG C/C++ front end](https://github.com/edgcpp/compiler).
It is a fork of [ccls](https://github.com/MaskRay/ccls) with the clang-based parts replaced:
the indexer, diagnostics, compile command handling, code completion, signature help, and formatting.
The index database, the pipeline, and the LSP request handlers are ccls's, so clients and
editor plugins written for ccls (including the `$ccls/*` extensions) work unchanged.

Features:

  * definition/references, declaration, implementation, type definition
  * hover with documentation comments
  * call, inheritance, and member hierarchies (`$ccls/call`, `$ccls/inheritance`, `$ccls/member`, `$ccls/vars`)
  * document and workspace symbols, folding ranges, semantic highlighting, preprocessor skipped regions
  * rename
  * diagnostics from the EDG front end, updated as you type
  * code completion and signature help (index-based; see below)
  * formatting via the `clang-format` executable

## How it works

EDG is built as a callable, multi-threaded library (`MAKE_FRONT_END_CALLABLE`, `MULTIPLE_THREAD_COMPILATION`).
edgls supplies the back end (`back_end()`), so each translation unit is parsed by `edg_main` on an indexer thread.

* **References.** The front end records every declaration and reference of a symbol for its cross-reference
  output (`--xref`). A small patch (`edg/patches/0001-symbol-reference-callback.patch`) exposes these records
  through a callback, including references in uninstantiated template bodies, implicit constructor/destructor
  calls, and macro uses.
* **Entities.** At the end of the translation unit, referenced entities are described from the IL: names,
  types, bases, overridden virtual functions, fields and their offsets. Template instances (`S<int>::get`)
  are mapped to the prototype instantiation of their template (`S<T>::get`), the way clang maps
  specializations to their patterns. Front end symbols of closed scopes may be freed by then, so this step
  uses only the IL, which stays valid for the back end.
* **Extents** come from the IL's declaration position supplements and the function-scope memory regions.
* **Diagnostics** use `--output_mode sarif`.
* **Compile commands.** `compile_commands.json` entries are GCC/Clang command lines. edgls queries the
  compiler (`-E -v -dM`) once per compiler and target to obtain its system include directories, version, and
  predefined macros, and runs the front end in the corresponding GNU or clang emulation mode.

`src/edg/` is the only code that includes EDG headers; the rest of the code base sees the plain C++ interface
in `src/edg/edg.hh`.

## Building

Requirements: an EDG front end checkout, LLVM (only the Support library is used), CMake 3.21, Python 3,
`patch`, and a C++17 compiler.

```sh
cmake -G Ninja -S . -B out/rel -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_PREFIX_PATH=/path/to/llvm/install \
  -DEDG_SOURCE_DIR=/path/to/edgcpp-compiler
ninja -C out/rel
```

`EDG_SOURCE_DIR` defaults to `../edgcpp-compiler`. The EDG sources are copied into the build directory and
the patches under `edg/patches/` are applied there; the checkout is not modified. `EDG_MACRO_CONF` selects
the EDG macro configuration (default: `linux-clang-release` with clang, `linux-gcc-release` otherwise).

Index tests: `./out/rel/edgls -test-index`. Set `CCLS_UPDATE_EXPECT=1` to regenerate expectations.

## Configuration

The initialization options are those of ccls, with one addition:

* `edg.baseDir`: the EDG base directory containing `lib/predefined_macros.txt`, used when the compiler of a
  compile command cannot be queried. Defaults to a directory generated at build time from the host compilers.
  Otherwise the compiler's predefined macros are written to `$XDG_CACHE_HOME/edgls/`.

`cache.directory` defaults to `.edgls-cache` so that the index files of ccls and edgls, which use different
USRs, are not mixed.

`clang.excludeArgs`, `clang.extraArgs`, `clang.prependArgs`, and `clang.pathMappings` apply to compile commands
as in ccls.

## Limitations

* Code completion and signature help are computed from the index rather than by the front end: members of
  the declared type of a variable, of a class or namespace before `::`, and names visible in the enclosing
  function, class, and globally. Expressions whose type is not recorded in the index (e.g. the result of a
  call through a template) are not completed.
* Objective-C is not supported.
* There are no fix-its or code actions.
* Template members are displayed with the template parameters of the prototype instantiation, e.g.
  `int S<T>::get()`.
* The front end always targets the host (x86-64 LP64 on Linux). `-m32`, `--target`, and `-march` only
  affect the predefined macros obtained from the compiler.
* Response files (`@file`) in compile commands are expanded; `compile_flags.txt` is not supported.
* Memory usage per translation unit is higher than clang's for template-heavy code; lower `index.threads` if
  needed.
