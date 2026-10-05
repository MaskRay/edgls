void called() {}

struct Foo {
  Foo();
};

Foo::Foo() {
  called();
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 2059144113310522824,
      "detailed_name": "Foo::Foo()",
      "qual_name_offset": 0,
      "short_name": "Foo",
      "spell": "7:6-7:9|7:1-9:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": ["8:3-8:9|9669741488085832541|3|36"],
      "kind": 9,
      "parent_kind": 23,
      "storage": 0,
      "declarations": ["4:3-4:6|4:3-4:8|1|-1"],
      "derived": [],
      "uses": []
    }, {
      "usr": 9669741488085832541,
      "detailed_name": "void called()",
      "qual_name_offset": 5,
      "short_name": "called",
      "spell": "1:6-1:12|1:1-1:17|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["8:3-8:9|36|-1"]
    }],
  "usr2type": [{
      "usr": 11569259296725656241,
      "detailed_name": "struct Foo {}",
      "qual_name_offset": 7,
      "short_name": "Foo",
      "spell": "3:8-3:11|3:1-5:2|2|-1",
      "bases": [],
      "funcs": [2059144113310522824],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["4:3-4:6|4|-1", "7:1-7:4|4|-1", "7:6-7:9|4|-1"]
    }],
  "usr2var": []
}
*/
