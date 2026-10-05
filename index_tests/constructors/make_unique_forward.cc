template <typename T, typename... Args> T *fake_make_unique(Args &&...args) {
  return new T(static_cast<Args &&>(args)...);
}

struct A {
  A();
  A(int);
  A(int, int);
};

void caller() {
  fake_make_unique<A>();
  fake_make_unique<A>(1);
  fake_make_unique<A>(1, 2);
}

// Each constructor gets an implicit use at its forwarding call site.

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 2951394154304786844,
      "detailed_name": "T *fake_make_unique(Args &&...args)",
      "qual_name_offset": 3,
      "short_name": "fake_make_unique",
      "spell": "1:44-1:60|1:41-3:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["12:3-12:19|36|-1", "13:3-13:19|36|-1", "14:3-14:19|36|-1"]
    }, {
      "usr": 3297231101384385958,
      "detailed_name": "A::A(int)",
      "qual_name_offset": 0,
      "short_name": "A",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 9,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["7:3-7:4|7:3-7:9|1|-1"],
      "derived": [],
      "uses": []
    }, {
      "usr": 4149031237663466556,
      "detailed_name": "A::A(int, int)",
      "qual_name_offset": 0,
      "short_name": "A",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 9,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["8:3-8:4|8:3-8:14|1|-1"],
      "derived": [],
      "uses": []
    }, {
      "usr": 11113268046312605898,
      "detailed_name": "void caller()",
      "qual_name_offset": 5,
      "short_name": "caller",
      "spell": "11:6-11:12|11:1-15:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": ["12:3-12:19|2951394154304786844|3|36", "13:3-13:19|2951394154304786844|3|36", "14:3-14:19|2951394154304786844|3|36"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 14857003228789881583,
      "detailed_name": "A::A()",
      "qual_name_offset": 0,
      "short_name": "A",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 9,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["6:3-6:4|6:3-6:6|1|-1"],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
      "usr": 12634755879794372453,
      "detailed_name": "Args",
      "qual_name_offset": 0,
      "short_name": "Args",
      "spell": "1:35-1:39|1:11-3:2|2|-1",
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
      "uses": ["1:61-1:65|4|-1"]
    }, {
      "usr": 17635323982382475078,
      "detailed_name": "struct A {}",
      "qual_name_offset": 7,
      "short_name": "A",
      "spell": "5:8-5:9|5:1-9:2|2|-1",
      "bases": [],
      "funcs": [14857003228789881583, 3297231101384385958, 4149031237663466556],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["6:3-6:4|4|-1", "7:3-7:4|4|-1", "8:3-8:4|4|-1", "12:20-12:21|4|-1", "13:20-13:21|4|-1", "14:20-14:21|4|-1"]
    }, {
      "usr": 17806111693037073392,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "1:20-1:21|1:11-1:21|2|-1",
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
      "uses": ["1:41-1:42|4|-1"]
    }],
  "usr2var": []
}
*/
