// Copyright 2017-2018 ccls Authors
// SPDX-License-Identifier: Apache-2.0

#include "sema_manager.hh"

#include "edg/edg.hh"
#include "edg_args.hh"
#include "log.hh"
#include "pipeline.hh"
#include "platform.hh"

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/CrashRecoveryContext.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/Threading.h>
#include <llvm/Support/raw_ostream.h>

#include <rapidjson/document.h>

#include <algorithm>
#include <chrono>
#include <thread>
namespace chrono = std::chrono;

using namespace llvm;

namespace ccls {
namespace {
std::string pathFromUri(StringRef uri) {
  if (!uri.consume_front("file://"))
    return uri.str();
  std::string ret;
  for (size_t i = 0; i < uri.size(); i++) {
    if (uri[i] == '%' && i + 2 < uri.size()) {
      unsigned v;
      if (!uri.substr(i + 1, 2).getAsInteger(16, v)) {
        ret += char(v);
        i += 2;
        continue;
      }
    }
    ret += uri[i];
  }
  return ret;
}

void fromLocation(const rapidjson::Value &loc, DiagBase &d) {
  auto pl = loc.FindMember("physicalLocation");
  if (pl == loc.MemberEnd() || !pl->value.IsObject())
    return;
  auto al = pl->value.FindMember("artifactLocation");
  if (al != pl->value.MemberEnd() && al->value.IsObject()) {
    auto uri = al->value.FindMember("uri");
    if (uri != al->value.MemberEnd() && uri->value.IsString())
      d.file = pathFromUri(uri->value.GetString());
  }
  auto region = pl->value.FindMember("region");
  if (region != pl->value.MemberEnd() && region->value.IsObject()) {
    int line = 1, column = 1;
    if (auto it = region->value.FindMember("startLine"); it != region->value.MemberEnd() && it->value.IsInt())
      line = it->value.GetInt();
    if (auto it = region->value.FindMember("startColumn"); it != region->value.MemberEnd() && it->value.IsInt())
      column = it->value.GetInt();
    d.range.start = {(uint16_t)std::max(line - 1, 0), (int16_t)std::max(column - 1, 0)};
    d.range.end = d.range.start;
  }
}

std::string messageOf(const rapidjson::Value &v) {
  auto m = v.FindMember("message");
  if (m != v.MemberEnd() && m->value.IsObject())
    if (auto t = m->value.FindMember("text"); t != m->value.MemberEnd() && t->value.IsString())
      return t->value.GetString();
  return "";
}

// Extend an empty range to cover the token at its start.
void extendToToken(Range &range, const std::string &content) {
  int off = getOffsetForPosition({range.start.line, range.start.column}, content);
  if (off < 0 || off >= (int)content.size())
    return;
  auto ident = [](char c) { return isalnum((unsigned char)c) || c == '_' || (unsigned char)c >= 0x80; };
  int end = off;
  if (ident(content[off]))
    while (end < (int)content.size() && ident(content[end]))
      end++;
  else if (content[off] != '\n')
    end++;
  int col = range.start.column;
  for (int i = off; i < end; i++)
    if (((unsigned char)content[i] & 0xc0) != 0x80)
      col++;
  range.end = {range.start.line, (int16_t)col};
}

void *diagnosticMain(void *manager_) {
  auto *manager = static_cast<SemaManager *>(manager_);
  set_thread_name("diag");
  while (true) {
    SemaManager::DiagTask task = manager->diag_tasks.dequeue();
    if (pipeline::g_quit.load(std::memory_order_relaxed))
      break;
    int64_t wait =
        task.wait_until -
        chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count();
    if (wait > 0)
      std::this_thread::sleep_for(chrono::duration<int64_t, std::milli>(std::min(wait, task.debounce)));

    Project::Entry entry = manager->project_->findEntry(task.path, false, false);
    EdgInvocation inv;
    if (!buildEdgInvocation(entry.args, task.path, inv))
      continue;
    edg::Request req;
    req.args = std::move(inv.args);
    req.index = false;
    std::string content;
    {
      std::lock_guard lock(manager->wfiles->mutex);
      for (auto &[path, wf] : manager->wfiles->files) {
        req.remapped.emplace_back(path, wf->buffer_content);
        if (path == task.path)
          content = wf->buffer_content;
      }
    }
    edg::Unit unit;
    {
      llvm::CrashRecoveryContext crc;
      if (!crc.RunSafely([&]() { edg::run(req, unit); })) {
        LOG_S(ERROR) << "the front end crashed for " << task.path;
      llvm::CrashRecoveryContext().RunSafely([]() { edg::recover(); });
        continue;
      }
    }

    std::vector<Diagnostic> ls_diags;
    for (Diag &d : parseSarif(unit.sarif)) {
      bool in_main = d.file == task.path;
      if (!in_main && d.level < DiagLevel::Error)
        continue;
      Diagnostic &ls = ls_diags.emplace_back();
      if (in_main) {
        extendToToken(d.range, content);
        ls.range = lsRange{{d.range.start.line, d.range.start.column}, {d.range.end.line, d.range.end.column}};
        ls.message = d.message;
      } else {
        // An error in an included file.
        ls.range = lsRange{{0, 0}, {0, 0}};
        ls.message = d.file + ":" + std::to_string(d.range.start.line + 1) + ":" +
                     std::to_string(d.range.start.column + 1) + ": " + d.message;
      }
      switch (d.level) {
      case DiagLevel::Ignored:
      case DiagLevel::Remark:
        ls.severity = 4;
        break;
      case DiagLevel::Note:
        ls.severity = 3;
        break;
      case DiagLevel::Warning:
        ls.severity = 2;
        break;
      default:
        ls.severity = 1;
        break;
      }
      ls.code = d.code;
      for (const Note &n : d.notes) {
        if (g_config->client.diagnosticsRelatedInformation && n.file.size()) {
          Location loc{DocumentUri::fromPath(n.file),
                       lsRange{{n.range.start.line, n.range.start.column}, {n.range.end.line, n.range.end.column}}};
          ls.relatedInformation.push_back({loc, n.message});
        } else {
          ls.message += "\n\n" + n.message;
        }
      }
    }

    {
      std::lock_guard lock(manager->wfiles->mutex);
      if (WorkingFile *wf = manager->wfiles->getFileUnlocked(task.path))
        wf->diagnostics = ls_diags;
    }
    manager->on_diagnostic_(task.path, ls_diags);
  }
  pipeline::threadLeave();
  return nullptr;
}
} // namespace

std::vector<Diag> parseSarif(const std::string &sarif) {
  std::vector<Diag> ret;
  // The output may contain several SARIF logs, e.g. when the front end was
  // restarted after a catastrophic error.
  for (size_t start = 0; (start = sarif.find("{\"version\"", start)) != std::string::npos;) {
    size_t next = sarif.find("{\"version\"", start + 1);
    std::string log = sarif.substr(start, next == std::string::npos ? std::string::npos : next - start);
    start = next;
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseTrailingCommasFlag | rapidjson::kParseStopWhenDoneFlag>(log.c_str());
    if (doc.HasParseError() || !doc.IsObject())
      continue;
    auto runs = doc.FindMember("runs");
    if (runs == doc.MemberEnd() || !runs->value.IsArray())
      continue;
    for (auto &run : runs->value.GetArray()) {
      auto results = run.FindMember("results");
      if (results == run.MemberEnd() || !results->value.IsArray())
        continue;
      for (auto &result : results->value.GetArray()) {
        Diag &d = ret.emplace_back();
        d.message = messageOf(result);
        if (auto it = result.FindMember("ruleId"); it != result.MemberEnd() && it->value.IsString()) {
          StringRef id = it->value.GetString();
          id.consume_front("EC");
          id.getAsInteger(10, d.code);
        }
        StringRef level = "error";
        if (auto it = result.FindMember("level"); it != result.MemberEnd() && it->value.IsString())
          level = it->value.GetString();
        d.level = level == "warning" ? DiagLevel::Warning
                  : level == "note"  ? DiagLevel::Remark
                  : level == "none"  ? DiagLevel::Remark
                                     : DiagLevel::Error;
        if (auto it = result.FindMember("locations");
            it != result.MemberEnd() && it->value.IsArray() && it->value.Size())
          fromLocation(it->value[0], d);
        if (auto it = result.FindMember("relatedLocations"); it != result.MemberEnd() && it->value.IsArray())
          for (auto &rel : it->value.GetArray()) {
            Note &n = d.notes.emplace_back();
            n.level = DiagLevel::Note;
            n.message = messageOf(rel);
            fromLocation(rel, n);
          }
      }
    }
  }
  return ret;
}

SemaManager::SemaManager(Project *project, WorkingFiles *wfiles, OnDiagnostic on_diagnostic, OnDropped on_dropped)
    : project_(project), wfiles(wfiles), on_diagnostic_(std::move(on_diagnostic)), on_dropped_(std::move(on_dropped)) {
  spawnThread(ccls::diagnosticMain, this);
}

void SemaManager::scheduleDiag(const std::string &path, int debounce) {
  static GroupMatch match(g_config->diagnostics.whitelist, g_config->diagnostics.blacklist);
  if (!match.matches(path))
    return;
  int64_t now =
      chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count();
  bool flag = false;
  {
    std::lock_guard lock(diag_mutex);
    int64_t &next = next_diag[path];
    auto &d = g_config->diagnostics;
    if (next <= now || now - next > std::max(d.onChange, std::max(d.onChange, d.onSave))) {
      next = now + debounce;
      flag = true;
    }
  }
  if (flag)
    diag_tasks.pushBack({path, now + debounce, debounce}, false);
}

void SemaManager::onView(const std::string &path) {
  bool seen;
  {
    std::lock_guard lock(diag_mutex);
    seen = next_diag.count(path);
  }
  if (!seen && g_config->diagnostics.onOpen >= 0)
    scheduleDiag(path, g_config->diagnostics.onOpen);
}

void SemaManager::onSave(const std::string &path) {
  if (g_config->diagnostics.onSave >= 0)
    scheduleDiag(path, g_config->diagnostics.onSave);
}

void SemaManager::onClose(const std::string &path) {
  std::lock_guard lock(diag_mutex);
  next_diag.erase(path);
}

void SemaManager::clear() {
  std::lock_guard lock(diag_mutex);
  next_diag.clear();
}

void SemaManager::quit() { diag_tasks.pushBack({}); }
} // namespace ccls
