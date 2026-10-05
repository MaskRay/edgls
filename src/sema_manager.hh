// Copyright 2017-2018 ccls Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "lsp.hh"
#include "position.hh"
#include "project.hh"
#include "threaded_queue.hh"
#include "working_files.hh"

#include <functional>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace ccls {
enum class DiagLevel : uint8_t { Ignored, Note, Remark, Warning, Error, Fatal };

struct DiagBase {
  Range range;
  std::string message;
  std::string file;
  DiagLevel level = DiagLevel::Note;
  int code = 0;
  bool concerned = false;
};
struct Note : DiagBase {};
struct Diag : DiagBase {
  std::vector<Note> notes;
  std::vector<TextEdit> edits;
};

// Parse the front end's SARIF output. Ranges are empty (start == end) and use
// 0-based lines and code point columns.
std::vector<Diag> parseSarif(const std::string &sarif);

struct SemaManager {
  using OnDiagnostic = std::function<void(std::string path, std::vector<Diagnostic> diagnostics)>;
  using OnDropped = std::function<void(RequestId request_id)>;

  struct DiagTask {
    std::string path;
    int64_t wait_until;
    int64_t debounce;
  };

  SemaManager(Project *project, WorkingFiles *wfiles, OnDiagnostic on_diagnostic, OnDropped on_dropped);

  void scheduleDiag(const std::string &path, int debounce);
  void onView(const std::string &path);
  void onSave(const std::string &path);
  void onClose(const std::string &path);
  void clear();
  void quit();

  Project *project_;
  WorkingFiles *wfiles;
  OnDiagnostic on_diagnostic_;
  OnDropped on_dropped_;

  std::mutex diag_mutex;
  std::unordered_map<std::string, int64_t> next_diag;
  ThreadedQueue<DiagTask> diag_tasks;
};

// Cached completion information, so we can give fast completion results when
// the user erases a character. vscode will resend the completion request if
// that happens.
template <typename T> struct CompleteConsumerCache {
  std::mutex mutex;
  std::string path;
  std::string line;
  Position position;
  T result;

  template <typename Fn> void withLock(Fn &&fn) {
    std::lock_guard lock(mutex);
    fn();
  }
  bool isCacheValid(const std::string &path, const std::string &line, Position position) {
    std::lock_guard lock(mutex);
    return this->position == position && this->path == path &&
           this->line.compare(0, position.character, line, 0, position.character) == 0;
  }
};
} // namespace ccls
