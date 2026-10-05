class Foo {
  Foo* member;
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
      "vars": [{
          "L": 17135586093725838119,
          "R": 0
        }],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [17135586093725838119],
      "uses": ["2:3-2:6|4|-1"]
    }],
  "usr2var": [{
      "usr": 17135586093725838119,
      "detailed_name": "Foo *Foo::member",
      "qual_name_offset": 5,
      "short_name": "member",
      "spell": "2:8-2:14|2:3-2:14|2|-1",
      "type": 11569259296725656241,
      "kind": 8,
      "parent_kind": 5,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
