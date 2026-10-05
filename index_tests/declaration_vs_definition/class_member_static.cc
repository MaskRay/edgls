class Foo {
  static int foo;
};

int Foo::foo;

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
      "uses": ["5:5-5:8|4|-1"]
    }],
  "usr2var": [{
      "usr": 17760892335118264467,
      "detailed_name": "static int Foo::foo",
      "qual_name_offset": 11,
      "short_name": "foo",
      "spell": "5:10-5:13|5:1-5:13|2|-1",
      "type": 0,
      "kind": 8,
      "parent_kind": 5,
      "storage": 0,
      "declarations": ["2:14-2:17|2:3-2:17|1|-1"],
      "uses": []
    }]
}
*/
