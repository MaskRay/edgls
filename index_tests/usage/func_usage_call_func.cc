void called() {}
void caller() {
  called();
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 9669741488085832541,
      "detailed_name": "void called()",
      "qual_name_offset": 5,
      "short_name": "called",
      "spell": "1:6-1:12|1:1-1:17|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["3:3-3:9|36|-1"]
    }, {
      "usr": 11113268046312605898,
      "detailed_name": "void caller()",
      "qual_name_offset": 5,
      "short_name": "caller",
      "spell": "2:6-2:12|2:1-4:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": ["3:3-3:9|9669741488085832541|3|36"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }],
  "usr2type": [],
  "usr2var": []
}
*/
