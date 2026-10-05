template<typename T>
void accept(T);

void foo() {
  accept(1);
  accept(true);
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 7711579111127724480,
      "detailed_name": "void accept(T)",
      "qual_name_offset": 5,
      "short_name": "accept",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["2:6-2:12|2:1-2:15|1|-1"],
      "derived": [],
      "uses": ["5:3-5:9|36|-1", "6:3-6:9|36|-1"]
    }, {
      "usr": 9608532274827022249,
      "detailed_name": "void foo()",
      "qual_name_offset": 5,
      "short_name": "foo",
      "spell": "4:6-4:9|4:1-7:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": ["5:3-5:9|7711579111127724480|3|36", "6:3-6:9|7711579111127724480|3|36"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
      "usr": 5904965689469894439,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "1:19-1:20|1:10-2:15|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 26,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["2:13-2:14|4|-1"]
    }],
  "usr2var": []
}
*/
