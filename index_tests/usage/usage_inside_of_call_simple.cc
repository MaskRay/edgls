void called(int a);

int gen() { return 1; }

void foo() {
  called(gen() * gen());
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 9608532274827022249,
      "detailed_name": "void foo()",
      "qual_name_offset": 5,
      "short_name": "foo",
      "spell": "5:6-5:9|5:1-7:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": ["6:18-6:21|16588592525380060745|3|36", "6:10-6:13|16588592525380060745|3|36", "6:3-6:9|12536051537729490967|3|36"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 12536051537729490967,
      "detailed_name": "void called(int a)",
      "qual_name_offset": 5,
      "short_name": "called",
      "bases": [],
      "vars": [10053615170980444378],
      "callees": [],
      "kind": 12,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["1:6-1:12|1:1-1:19|1|-1"],
      "derived": [],
      "uses": ["6:3-6:9|36|-1"]
    }, {
      "usr": 16588592525380060745,
      "detailed_name": "int gen()",
      "qual_name_offset": 4,
      "short_name": "gen",
      "spell": "3:5-3:8|3:1-3:24|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["6:18-6:21|36|-1", "6:10-6:13|36|-1"]
    }],
  "usr2type": [],
  "usr2var": [{
      "usr": 10053615170980444378,
      "detailed_name": "int a",
      "qual_name_offset": 4,
      "short_name": "a",
      "spell": "1:17-1:18|1:13-1:18|2|-1",
      "type": 0,
      "kind": 253,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
