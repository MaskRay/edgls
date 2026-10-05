struct Foo {
  static int x;
};

void accept(int);

void foo() {
  accept(Foo::x);
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 8448580434163129902,
      "detailed_name": "void accept(int)",
      "qual_name_offset": 5,
      "short_name": "accept",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["5:6-5:12|5:1-5:17|1|-1"],
      "derived": [],
      "uses": ["8:3-8:9|36|-1"]
    }, {
      "usr": 9608532274827022249,
      "detailed_name": "void foo()",
      "qual_name_offset": 5,
      "short_name": "foo",
      "spell": "7:6-7:9|7:1-9:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": ["8:3-8:9|8448580434163129902|3|36"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
      "usr": 11569259296725656241,
      "detailed_name": "struct Foo {}",
      "qual_name_offset": 7,
      "short_name": "Foo",
      "spell": "1:8-1:11|1:1-3:2|2|-1",
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
      "uses": ["8:10-8:13|4|-1"]
    }],
  "usr2var": [{
      "usr": 13262920552766832354,
      "detailed_name": "static int Foo::x",
      "qual_name_offset": 11,
      "short_name": "x",
      "type": 0,
      "kind": 8,
      "parent_kind": 0,
      "storage": 1,
      "declarations": ["2:14-2:15|2:3-2:15|1|-1"],
      "uses": ["8:15-8:16|12|-1"]
    }]
}
*/
