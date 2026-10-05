class Foo {
  static int member;
};
/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [],
  "usr2type": [{
      "usr": 11569259296725656241,
      "detailed_name": "class Foo {}",
      "qual_name_offset": 6,
      "short_name": "Foo",
      "spell": "1:7-1:10|1:1-3:2|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": []
    }],
  "usr2var": [{
      "usr": 17135586093725838119,
      "detailed_name": "static int Foo::member",
      "qual_name_offset": 11,
      "short_name": "member",
      "type": 0,
      "kind": 8,
      "parent_kind": 0,
      "storage": 1,
      "declarations": ["2:14-2:20|2:3-2:20|1|-1"],
      "uses": []
    }]
}
*/
