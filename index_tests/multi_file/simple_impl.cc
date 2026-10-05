#include "simple_header.h"

void impl() {
  header();
}

/*
OUTPUT: simple_header.h
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 5998441837958845839,
      "detailed_name": "void header()",
      "qual_name_offset": 5,
      "short_name": "header",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["3:6-3:12|3:1-3:14|1|-1"],
      "derived": [],
      "uses": []
    }],
  "usr2type": [],
  "usr2var": []
}
OUTPUT: simple_impl.cc
{
  "includes": [{
      "line": 0,
      "resolved_path": "&simple_header.h"
    }],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 5998441837958845839,
      "detailed_name": "void header()",
      "qual_name_offset": 5,
      "short_name": "header",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 0,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["4:3-4:9|36|-1"]
    }, {
      "usr": 14539244003067466602,
      "detailed_name": "void impl()",
      "qual_name_offset": 5,
      "short_name": "impl",
      "spell": "3:6-3:10|3:1-5:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": ["4:3-4:9|5998441837958845839|3|36"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }],
  "usr2type": [],
  "usr2var": []
}
*/
