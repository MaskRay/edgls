struct T {};

extern T t;
/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [],
  "usr2type": [{
      "usr": 1983440246602941315,
      "detailed_name": "struct T {}",
      "qual_name_offset": 7,
      "short_name": "T",
      "spell": "1:8-1:9|1:1-1:12|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [7204641611859825469],
      "uses": ["3:8-3:9|4|-1"]
    }],
  "usr2var": [{
      "usr": 7204641611859825469,
      "detailed_name": "extern T t",
      "qual_name_offset": 9,
      "short_name": "t",
      "type": 1983440246602941315,
      "kind": 13,
      "parent_kind": 0,
      "storage": 1,
      "declarations": ["3:10-3:11|3:1-3:11|1|-1"],
      "uses": []
    }]
}
*/
