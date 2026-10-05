#define MACRO_CALL(e) e

bool called(bool a, bool b);

void caller() {
  MACRO_CALL(called(true, true));
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 11113268046312605898,
      "detailed_name": "void caller()",
      "qual_name_offset": 5,
      "short_name": "caller",
      "spell": "5:6-5:12|5:1-7:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": ["6:3-6:13|15272976263498525318|3|36"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 15272976263498525318,
      "detailed_name": "bool called(bool a, bool b)",
      "qual_name_offset": 5,
      "short_name": "called",
      "bases": [],
      "vars": [17235914947843223097, 925788267249540902],
      "callees": [],
      "kind": 12,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["3:6-3:12|3:1-3:28|1|-1"],
      "derived": [],
      "uses": ["6:3-6:13|36|-1"]
    }],
  "usr2type": [],
  "usr2var": [{
      "usr": 925788267249540902,
      "detailed_name": "bool b",
      "qual_name_offset": 5,
      "short_name": "b",
      "spell": "3:26-3:27|3:21-3:27|2|-1",
      "type": 0,
      "kind": 253,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 3808408915022256886,
      "detailed_name": "MACRO_CALL",
      "qual_name_offset": 0,
      "short_name": "MACRO_CALL",
      "hover": "#define MACRO_CALL(e) e",
      "spell": "1:9-1:19|1:1-1:24|2|-1",
      "type": 0,
      "kind": 255,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": ["6:3-6:13|4|-1"]
    }, {
      "usr": 17235914947843223097,
      "detailed_name": "bool a",
      "qual_name_offset": 5,
      "short_name": "a",
      "spell": "3:18-3:19|3:13-3:19|2|-1",
      "type": 0,
      "kind": 253,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/