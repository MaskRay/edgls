struct S {};
using F = S;
void Foo() {
  F a;
}

// TODO: Should we also add a usage to |S|?

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 2213156063291502229,
      "detailed_name": "void Foo()",
      "qual_name_offset": 5,
      "short_name": "Foo",
      "spell": "3:6-3:9|3:1-5:2|2|-1",
      "bases": [],
      "vars": [11534753538847192061],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
      "usr": 3795401214890978402,
      "detailed_name": "struct S {}",
      "qual_name_offset": 7,
      "short_name": "S",
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
      "instances": [],
      "uses": ["2:11-2:12|4|-1"]
    }, {
      "usr": 12099574315794256726,
      "detailed_name": "using F = S",
      "qual_name_offset": 6,
      "short_name": "F",
      "spell": "2:7-2:8|2:1-2:12|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 3795401214890978402,
      "kind": 252,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [11534753538847192061],
      "uses": ["4:3-4:4|4|-1"]
    }],
  "usr2var": [{
      "usr": 11534753538847192061,
      "detailed_name": "F a",
      "qual_name_offset": 2,
      "short_name": "a",
      "spell": "4:5-4:6|4:3-4:6|2|-1",
      "type": 12099574315794256726,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
