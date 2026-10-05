void called();

template <typename T>
void caller() {
  called();
}

void foo() {
  caller<int>();
}

/*
// NOTE: without caller<int>() instantation caller() is never visited so
// called() is never referenced.
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 9608532274827022249,
      "detailed_name": "void foo()",
      "qual_name_offset": 5,
      "short_name": "foo",
      "spell": "8:6-8:9|8:1-10:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": ["9:3-9:9|11113268046312605898|3|36"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 9669741488085832541,
      "detailed_name": "void called()",
      "qual_name_offset": 5,
      "short_name": "called",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["1:6-1:12|1:1-1:14|1|-1"],
      "derived": [],
      "uses": []
    }, {
      "usr": 11113268046312605898,
      "detailed_name": "void caller()",
      "qual_name_offset": 5,
      "short_name": "caller",
      "spell": "4:6-4:12|4:1-6:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["9:3-9:9|36|-1"]
    }],
  "usr2type": [{
      "usr": 3980682406575916263,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "3:20-3:21|3:11-6:2|2|-1",
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
      "uses": []
    }],
  "usr2var": []
}
*/