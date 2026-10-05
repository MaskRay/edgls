namespace hello {
void foo(int a, int b);
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 1200282760160601345,
      "detailed_name": "void hello::foo(int a, int b)",
      "qual_name_offset": 5,
      "short_name": "foo",
      "bases": [],
      "vars": [15154432793067918594, 7666106480796533710],
      "callees": [],
      "kind": 12,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["2:6-2:9|2:1-2:23|1|-1"],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
      "usr": 9568013184638781012,
      "detailed_name": "namespace hello {}",
      "qual_name_offset": 10,
      "short_name": "hello",
      "bases": [],
      "funcs": [1200282760160601345],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 3,
      "parent_kind": 0,
      "declarations": ["1:11-1:16|1:1-3:2|1|-1"],
      "derived": [],
      "instances": [],
      "uses": []
    }],
  "usr2var": [{
      "usr": 7666106480796533710,
      "detailed_name": "int b",
      "qual_name_offset": 4,
      "short_name": "b",
      "spell": "2:21-2:22|2:17-2:22|2|-1",
      "type": 0,
      "kind": 253,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 15154432793067918594,
      "detailed_name": "int a",
      "qual_name_offset": 4,
      "short_name": "a",
      "spell": "2:14-2:15|2:10-2:15|2|-1",
      "type": 0,
      "kind": 253,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
