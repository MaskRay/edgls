struct Type {};
static Type t;
/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [],
  "usr2type": [{
      "usr": 15323574594624129250,
      "detailed_name": "struct Type {}",
      "qual_name_offset": 7,
      "short_name": "Type",
      "spell": "1:8-1:12|1:1-1:15|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [16843970162298630736],
      "uses": ["2:8-2:12|4|-1"]
    }],
  "usr2var": [{
      "usr": 16843970162298630736,
      "detailed_name": "static Type t",
      "qual_name_offset": 12,
      "short_name": "t",
      "spell": "2:13-2:14|2:1-2:14|2|-1",
      "type": 15323574594624129250,
      "kind": 13,
      "parent_kind": 1,
      "storage": 2,
      "declarations": [],
      "uses": []
    }]
}
*/
