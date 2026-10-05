void consume(void (*)()) {}

void used() {}

void user() {
  void (*x)() = &used;
  consume(&used);
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 5910123705777270338,
      "detailed_name": "void consume(void (*)())",
      "qual_name_offset": 5,
      "short_name": "consume",
      "spell": "1:6-1:13|1:1-1:28|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["7:3-7:10|36|-1"]
    }, {
      "usr": 9208708212483644494,
      "detailed_name": "void user()",
      "qual_name_offset": 5,
      "short_name": "user",
      "spell": "5:6-5:10|5:1-8:2|2|-1",
      "bases": [],
      "vars": [14146831697717891378],
      "callees": ["6:18-6:22|14480149058067033119|3|132", "7:12-7:16|14480149058067033119|3|132", "7:3-7:10|5910123705777270338|3|36"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 14480149058067033119,
      "detailed_name": "void used()",
      "qual_name_offset": 5,
      "short_name": "used",
      "spell": "3:6-3:10|3:1-3:15|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["6:18-6:22|132|-1", "7:12-7:16|132|-1"]
    }],
  "usr2type": [],
  "usr2var": [{
      "usr": 14146831697717891378,
      "detailed_name": "void (*x)()",
      "qual_name_offset": 7,
      "short_name": "x",
      "hover": "void (*x)() = &used",
      "spell": "6:10-6:11|6:3-6:22|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
