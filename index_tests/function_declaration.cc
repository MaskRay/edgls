void foo(int a, int b);

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 9186792343463881929,
      "detailed_name": "void foo(int a, int b)",
      "qual_name_offset": 5,
      "short_name": "foo",
      "bases": [],
      "vars": [2601837881964534718, 3601967938590420697],
      "callees": [],
      "kind": 12,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["1:6-1:9|1:1-1:23|1|-1"],
      "derived": [],
      "uses": []
    }],
  "usr2type": [],
  "usr2var": [{
      "usr": 2601837881964534718,
      "detailed_name": "int a",
      "qual_name_offset": 4,
      "short_name": "a",
      "spell": "1:14-1:15|1:10-1:15|2|-1",
      "type": 0,
      "kind": 253,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 3601967938590420697,
      "detailed_name": "int b",
      "qual_name_offset": 4,
      "short_name": "b",
      "spell": "1:21-1:22|1:17-1:22|2|-1",
      "type": 0,
      "kind": 253,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
