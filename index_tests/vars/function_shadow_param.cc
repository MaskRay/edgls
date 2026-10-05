void foo(int p) {
  { int p = 0; }
}
/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 10911161088781653026,
      "detailed_name": "void foo(int p)",
      "qual_name_offset": 5,
      "short_name": "foo",
      "spell": "1:6-1:9|1:1-3:2|2|-1",
      "bases": [],
      "vars": [9495093592066979275, 12836860067515936242],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }],
  "usr2type": [],
  "usr2var": [{
      "usr": 9495093592066979275,
      "detailed_name": "int p",
      "qual_name_offset": 4,
      "short_name": "p",
      "spell": "1:14-1:15|1:10-1:15|2|-1",
      "type": 0,
      "kind": 253,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 12836860067515936242,
      "detailed_name": "int p",
      "qual_name_offset": 4,
      "short_name": "p",
      "hover": "int p = 0",
      "spell": "2:9-2:10|2:5-2:14|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
