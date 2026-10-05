struct Foo {
  void foo();
};

void usage() {
  Foo* f = nullptr;
  f->foo();
}
/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 15590769424229303502,
      "detailed_name": "void usage()",
      "qual_name_offset": 5,
      "short_name": "usage",
      "spell": "5:6-5:11|5:1-8:2|2|-1",
      "bases": [],
      "vars": [9486852943735092107],
      "callees": ["7:6-7:9|17801912395874338981|3|36"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 17801912395874338981,
      "detailed_name": "void Foo::foo()",
      "qual_name_offset": 5,
      "short_name": "foo",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 6,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["2:8-2:11|2:3-2:13|1|-1"],
      "derived": [],
      "uses": ["7:6-7:9|36|-1"]
    }],
  "usr2type": [{
      "usr": 11569259296725656241,
      "detailed_name": "struct Foo {}",
      "qual_name_offset": 7,
      "short_name": "Foo",
      "spell": "1:8-1:11|1:1-3:2|2|-1",
      "bases": [],
      "funcs": [17801912395874338981],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [9486852943735092107],
      "uses": ["6:3-6:6|4|-1"]
    }],
  "usr2var": [{
      "usr": 9486852943735092107,
      "detailed_name": "Foo *f",
      "qual_name_offset": 5,
      "short_name": "f",
      "hover": "Foo *f = nullptr",
      "spell": "6:8-6:9|6:3-6:19|2|-1",
      "type": 11569259296725656241,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": ["7:3-7:4|12|-1"]
    }]
}
*/
