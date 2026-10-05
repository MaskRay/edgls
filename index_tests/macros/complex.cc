#define FOO(aaa, bbb) \
  int a();\
  int a() { return aaa + bbb; }


int make1() {
  return 3;
}
const int make2 = 5;


FOO(make1(), make2);

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 8917140797287640201,
      "detailed_name": "int a()",
      "qual_name_offset": 4,
      "short_name": "a",
      "spell": "12:1-12:4|12:1-12:4|2|-1",
      "bases": [],
      "vars": [],
      "callees": ["12:1-12:4|11119574266751322774|3|36"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 11119574266751322774,
      "detailed_name": "int make1()",
      "qual_name_offset": 4,
      "short_name": "make1",
      "spell": "6:5-6:10|6:1-8:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["12:1-12:4|36|-1"]
    }],
  "usr2type": [],
  "usr2var": [{
      "usr": 17915766478539034828,
      "detailed_name": "static const int make2",
      "qual_name_offset": 17,
      "short_name": "make2",
      "hover": "static const int make2 = 5",
      "spell": "9:11-9:16|9:1-9:20|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 1,
      "storage": 2,
      "declarations": [],
      "uses": ["12:1-12:4|12|-1"]
    }, {
      "usr": 18043316707112767911,
      "detailed_name": "FOO",
      "qual_name_offset": 0,
      "short_name": "FOO",
      "hover": "#define FOO(aaa, bbb) \\\n  int a();\\\n  int a() { return aaa + bbb; }",
      "spell": "1:9-1:12|1:1-3:32|2|-1",
      "type": 0,
      "kind": 255,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": ["12:1-12:4|4|-1"]
    }]
}
*/