static int helper() {
  return 5;
}

class Foo {
  int x = helper();
};

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 12639959675847837540,
      "detailed_name": "static int helper()",
      "qual_name_offset": 11,
      "short_name": "helper",
      "spell": "1:12-1:18|1:1-3:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 2,
      "declarations": [],
      "derived": [],
      "uses": ["6:11-6:17|36|-1"]
    }],
  "usr2type": [{
      "usr": 11569259296725656241,
      "detailed_name": "class Foo {}",
      "qual_name_offset": 6,
      "short_name": "Foo",
      "spell": "5:7-5:10|5:1-7:2|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [{
          "L": 13262920552766832354,
          "R": 0
        }],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": []
    }],
  "usr2var": [{
      "usr": 13262920552766832354,
      "detailed_name": "int Foo::x",
      "qual_name_offset": 4,
      "short_name": "x",
      "hover": "int Foo::x = helper()",
      "spell": "6:7-6:8|6:3-6:19|2|-1",
      "type": 0,
      "kind": 8,
      "parent_kind": 5,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
