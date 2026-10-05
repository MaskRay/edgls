struct Wrapper {
  Wrapper(int i);
};

int called() { return 1; }

Wrapper caller() {
  return called();
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 5173553709568002403,
      "detailed_name": "Wrapper::Wrapper(int i)",
      "qual_name_offset": 0,
      "short_name": "Wrapper",
      "bases": [],
      "vars": [14558428903463327561],
      "callees": [],
      "kind": 9,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["2:3-2:10|2:3-2:17|1|-1"],
      "derived": [],
      "uses": ["8:10-8:16|292|-1"]
    }, {
      "usr": 6134292750584075649,
      "detailed_name": "Wrapper caller()",
      "qual_name_offset": 8,
      "short_name": "caller",
      "spell": "7:9-7:15|7:1-9:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": ["8:10-8:16|9435824487658497577|3|36", "8:10-8:16|5173553709568002403|3|292"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 9435824487658497577,
      "detailed_name": "int called()",
      "qual_name_offset": 4,
      "short_name": "called",
      "spell": "5:5-5:11|5:1-5:27|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["8:10-8:16|36|-1"]
    }],
  "usr2type": [{
      "usr": 316561212496113048,
      "detailed_name": "struct Wrapper {}",
      "qual_name_offset": 7,
      "short_name": "Wrapper",
      "spell": "1:8-1:15|1:1-3:2|2|-1",
      "bases": [],
      "funcs": [5173553709568002403],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["2:3-2:10|4|-1", "7:1-7:8|4|-1"]
    }],
  "usr2var": [{
      "usr": 14558428903463327561,
      "detailed_name": "int i",
      "qual_name_offset": 4,
      "short_name": "i",
      "spell": "2:15-2:16|2:11-2:16|2|-1",
      "type": 0,
      "kind": 253,
      "parent_kind": 9,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
