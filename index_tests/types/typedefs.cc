typedef int (func)(const int *a, const int *b);
static func	g;

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 5106145208893044771,
      "detailed_name": "static func g",
      "qual_name_offset": 12,
      "short_name": "g",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 0,
      "storage": 2,
      "declarations": ["2:13-2:14|2:1-2:14|1|-1"],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
      "usr": 2033438264177251928,
      "detailed_name": "typedef int (const int *, const int *) func",
      "qual_name_offset": 39,
      "short_name": "func",
      "spell": "1:14-1:18|1:1-1:47|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 252,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["2:8-2:12|4|-1"]
    }],
  "usr2var": []
}
*/