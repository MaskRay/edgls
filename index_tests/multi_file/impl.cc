#include "header.h"

void Impl() {
  Foo1<int>();
}

/*
OUTPUT: header.h
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 6758873709578071575,
      "detailed_name": "void Foo1()",
      "qual_name_offset": 5,
      "short_name": "Foo1",
      "spell": "10:6-10:10|10:1-10:15|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
      "usr": 7061811758925486510,
      "detailed_name": "struct Base {}",
      "qual_name_offset": 7,
      "short_name": "Base",
      "spell": "3:8-3:12|3:1-3:15|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [10498319274618568453],
      "instances": [],
      "uses": ["5:26-5:30|4|-1"]
    }, {
      "usr": 10227484827946204421,
      "detailed_name": "struct Foo2 {}",
      "qual_name_offset": 7,
      "short_name": "Foo2",
      "spell": "13:8-13:12|13:1-13:15|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": []
    }, {
      "usr": 10498319274618568453,
      "detailed_name": "struct SameFileDerived : Base {}",
      "qual_name_offset": 7,
      "short_name": "SameFileDerived",
      "spell": "5:8-5:23|5:1-5:33|2|-1",
      "bases": [7061811758925486510],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["7:14-7:29|4|-1"]
    }, {
      "usr": 11624255805800798360,
      "detailed_name": "enum Foo3 {}",
      "qual_name_offset": 5,
      "short_name": "Foo3",
      "spell": "15:6-15:10|15:1-15:22|2|-1",
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
      "kind": 10,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": []
    }, {
      "usr": 14150670434914690909,
      "detailed_name": "using Foo0 = SameFileDerived",
      "qual_name_offset": 6,
      "short_name": "Foo0",
      "spell": "7:7-7:11|7:1-7:29|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 10498319274618568453,
      "kind": 252,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": []
    }, {
      "usr": 14280104040796946454,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "12:20-12:21|12:11-13:15|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 26,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": []
    }, {
      "usr": 16288653272220387881,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "9:20-9:21|9:11-10:15|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 26,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": []
    }],
  "usr2var": [{
      "usr": 1057647907449850394,
      "detailed_name": "int Foo4",
      "qual_name_offset": 4,
      "short_name": "Foo4",
      "spell": "17:5-17:9|17:1-17:9|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 3220350714530089128,
      "detailed_name": "static int Foo5",
      "qual_name_offset": 11,
      "short_name": "Foo5",
      "spell": "18:12-18:16|18:1-18:16|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 1,
      "storage": 2,
      "declarations": [],
      "uses": []
    }, {
      "usr": 5761508020785540753,
      "detailed_name": "C",
      "qual_name_offset": 0,
      "short_name": "C",
      "spell": "15:19-15:20|15:19-15:20|2|-1",
      "type": 11624255805800798360,
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
      "spell": "15:13-15:14|15:13-15:14|2|-1",
      "type": 11624255805800798360,
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
      "spell": "15:16-15:17|15:16-15:17|2|-1",
      "type": 11624255805800798360,
      "kind": 22,
      "parent_kind": 10,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
OUTPUT: impl.cc
{
  "includes": [{
      "line": 0,
      "resolved_path": "&header.h"
    }],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 2080108969257566337,
      "detailed_name": "void Impl()",
      "qual_name_offset": 5,
      "short_name": "Impl",
      "spell": "3:6-3:10|3:1-5:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": ["4:3-4:7|6758873709578071575|3|36"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 6758873709578071575,
      "detailed_name": "void Foo1()",
      "qual_name_offset": 5,
      "short_name": "Foo1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 0,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["4:3-4:7|36|-1"]
    }],
  "usr2type": [],
  "usr2var": []
}
*/
