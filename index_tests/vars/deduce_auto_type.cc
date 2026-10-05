class Foo {};
void f() {
  auto x = new Foo();
  auto* y = new Foo();
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 8041731272366646527,
      "detailed_name": "void f()",
      "qual_name_offset": 5,
      "short_name": "f",
      "spell": "2:6-2:7|2:1-5:2|2|-1",
      "bases": [],
      "vars": [12975232541531700067, 15066691948599932917],
      "callees": ["3:12-3:15|13428682614327868979|3|36", "3:12-3:15|10185684319667591300|3|36", "4:13-4:16|13428682614327868979|3|36", "4:13-4:16|10185684319667591300|3|36"],
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
      "uses": ["3:12-3:15|36|-1", "4:13-4:16|36|-1"]
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
      "uses": ["3:12-3:15|36|-1", "4:13-4:16|36|-1"]
    }],
  "usr2type": [{
      "usr": 11569259296725656241,
      "detailed_name": "class Foo {}",
      "qual_name_offset": 6,
      "short_name": "Foo",
      "spell": "1:7-1:10|1:1-1:13|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [12975232541531700067, 15066691948599932917],
      "uses": ["3:16-3:19|4|-1", "4:17-4:20|4|-1"]
    }],
  "usr2var": [{
      "usr": 12975232541531700067,
      "detailed_name": "Foo *x",
      "qual_name_offset": 5,
      "short_name": "x",
      "hover": "Foo *x = new Foo()",
      "spell": "3:8-3:9|3:3-3:21|2|-1",
      "type": 11569259296725656241,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 15066691948599932917,
      "detailed_name": "Foo *y",
      "qual_name_offset": 5,
      "short_name": "y",
      "hover": "Foo *y = new Foo()",
      "spell": "4:9-4:10|4:3-4:22|2|-1",
      "type": 11569259296725656241,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
