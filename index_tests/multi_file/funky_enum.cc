enum Foo {
#include "funky_enum.h"
};

/*
// TODO: In the future try to have better support for types defined across
// multiple files.

OUTPUT: funky_enum.h
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [],
  "usr2type": [{
      "usr": 11569259296725656241,
      "detailed_name": "",
      "qual_name_offset": 0,
      "short_name": "",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [{
          "L": 6742167846147047236,
          "R": -1
        }, {
          "L": 12484003124849472749,
          "R": -1
        }, {
          "L": 5761508020785540753,
          "R": -1
        }],
      "alias_of": 0,
      "kind": 0,
      "parent_kind": 0,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": []
    }],
  "usr2var": [{
      "usr": 5761508020785540753,
      "detailed_name": "C",
      "qual_name_offset": 0,
      "short_name": "C",
      "spell": "6:1-6:2|6:1-6:2|2|-1",
      "type": 11569259296725656241,
      "kind": 22,
      "parent_kind": 10,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 6742167846147047236,
      "detailed_name": "A",
      "qual_name_offset": 0,
      "short_name": "A",
      "spell": "4:1-4:2|4:1-4:2|2|-1",
      "type": 11569259296725656241,
      "kind": 22,
      "parent_kind": 10,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 12484003124849472749,
      "detailed_name": "B",
      "qual_name_offset": 0,
      "short_name": "B",
      "spell": "5:1-5:2|5:1-5:2|2|-1",
      "type": 11569259296725656241,
      "kind": 22,
      "parent_kind": 10,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
OUTPUT: funky_enum.cc
{
  "includes": [{
      "line": 1,
      "resolved_path": "&funky_enum.h"
    }],
  "skipped_ranges": [],
  "usr2func": [],
  "usr2type": [{
      "usr": 11569259296725656241,
      "detailed_name": "enum Foo {}",
      "qual_name_offset": 5,
      "short_name": "Foo",
      "spell": "1:6-1:9|1:1-3:2|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 10,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": []
    }],
  "usr2var": []
}
*/