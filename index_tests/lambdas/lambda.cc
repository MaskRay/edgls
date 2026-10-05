void foo() {
  int x;

  auto dosomething = [&x](int y) {
    ++x;
    ++y;
  };

  dosomething(1);
  dosomething(1);
  dosomething(1);
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 5559927272543330341,
      "detailed_name": "operator()()",
      "qual_name_offset": 0,
      "short_name": "operator()",
      "spell": "4:26-4:27|4:3-4:32|2|-1",
      "bases": [],
      "vars": [14743649039289126827],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["9:14-9:15|36|-1", "10:14-10:15|36|-1", "11:14-11:15|36|-1"]
    }, {
      "usr": 9608532274827022249,
      "detailed_name": "void foo()",
      "qual_name_offset": 5,
      "short_name": "foo",
      "spell": "1:6-1:9|1:1-12:2|2|-1",
      "bases": [],
      "vars": [15058320962454244391, 6760724006777900910],
      "callees": ["9:14-9:15|5559927272543330341|3|36", "10:14-10:15|5559927272543330341|3|36", "11:14-11:15|5559927272543330341|3|36"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }],
  "usr2type": [],
  "usr2var": [{
      "usr": 6760724006777900910,
      "detailed_name": "(lambda) dosomething",
      "qual_name_offset": 9,
      "short_name": "dosomething",
      "hover": "(lambda) dosomething = [&x](int y) {\n    ++x;\n    ++y;\n  }",
      "spell": "4:8-4:19|4:3-7:4|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": ["9:3-9:14|132|-1", "10:3-10:14|132|-1", "11:3-11:14|132|-1"]
    }, {
      "usr": 14743649039289126827,
      "detailed_name": "int y",
      "qual_name_offset": 4,
      "short_name": "y",
      "spell": "4:31-4:32|4:27-4:32|2|-1",
      "type": 0,
      "kind": 253,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": ["6:7-6:8|28|-1"]
    }, {
      "usr": 15058320962454244391,
      "detailed_name": "int x",
      "qual_name_offset": 4,
      "short_name": "x",
      "spell": "2:7-2:8|2:3-2:8|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": ["5:7-5:8|28|-1", "4:24-4:25|132|-1"]
    }]
}
*/
