class Foo {
public:
  Foo() {}
};

void foo() {
  Foo f;
  Foo* f2 = new Foo();
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
      "spell": "3:3-3:6|3:3-3:11|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 9,
      "parent_kind": 5,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["7:7-7:8|292|-1", "8:17-8:20|292|-1"]
    }, {
      "usr": 9608532274827022249,
      "detailed_name": "void foo()",
      "qual_name_offset": 5,
      "short_name": "foo",
      "spell": "6:6-6:9|6:1-9:2|2|-1",
      "bases": [],
      "vars": [13632787749891303300, 15342871659514321518],
      "callees": ["7:7-7:8|2059144113310522824|3|292", "8:13-8:16|13428682614327868979|3|36", "8:13-8:16|10185684319667591300|3|36", "8:17-8:20|2059144113310522824|3|292"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 10185684319667591300,
      "detailed_name": "void *operator new(unsigned long)",
      "qual_name_offset": 6,
      "short_name": "operator new",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 0,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["8:13-8:16|36|-1"]
    }, {
      "usr": 13428682614327868979,
      "detailed_name": "void operator delete(void *, unsigned long) noexcept",
      "qual_name_offset": 5,
      "short_name": "operator delete",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 0,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["8:13-8:16|36|-1"]
    }],
  "usr2type": [{
      "usr": 11569259296725656241,
      "detailed_name": "class Foo {}",
      "qual_name_offset": 6,
      "short_name": "Foo",
      "spell": "1:7-1:10|1:1-4:2|2|-1",
      "bases": [],
      "funcs": [2059144113310522824],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [13632787749891303300, 15342871659514321518],
      "uses": ["3:3-3:6|4|-1", "7:3-7:6|4|-1", "8:3-8:6|4|-1", "8:17-8:20|4|-1"]
    }],
  "usr2var": [{
      "usr": 13632787749891303300,
      "detailed_name": "Foo f",
      "qual_name_offset": 4,
      "short_name": "f",
      "spell": "7:7-7:8|7:3-7:8|2|-1",
      "type": 11569259296725656241,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 15342871659514321518,
      "detailed_name": "Foo *f2",
      "qual_name_offset": 5,
      "short_name": "f2",
      "hover": "Foo *f2 = new Foo()",
      "spell": "8:8-8:10|8:3-8:22|2|-1",
      "type": 11569259296725656241,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
