enum A {};
enum B {};

template<typename T>
struct Foo {
  struct Inner {};
};

Foo<A>::Inner a;
Foo<B> b;
/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [],
  "usr2type": [{
      "usr": 159242453973086545,
      "detailed_name": "struct Foo {}",
      "qual_name_offset": 7,
      "short_name": "Foo",
      "spell": "5:8-5:11|5:1-7:2|2|-1",
      "bases": [],
      "funcs": [],
      "types": [3320579157636203467],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [664645370489364250],
      "uses": ["9:1-9:4|4|-1", "10:1-10:4|4|-1"]
    }, {
      "usr": 3320579157636203467,
      "detailed_name": "struct Foo<T>::Inner {}",
      "qual_name_offset": 7,
      "short_name": "Inner",
      "spell": "6:10-6:15|6:3-6:18|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 23,
      "declarations": [],
      "derived": [],
      "instances": [8659751582206604765],
      "uses": ["9:9-9:14|4|-1"]
    }, {
      "usr": 7410003400252181498,
      "detailed_name": "enum B {}",
      "qual_name_offset": 5,
      "short_name": "B",
      "spell": "2:6-2:7|2:1-2:10|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 10,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["10:5-10:6|4|-1"]
    }, {
      "usr": 7701195210376285427,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "4:19-4:20|4:10-7:2|2|-1",
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
    }, {
      "usr": 17635323982382475078,
      "detailed_name": "enum A {}",
      "qual_name_offset": 5,
      "short_name": "A",
      "spell": "1:6-1:7|1:1-1:10|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 10,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["9:5-9:6|4|-1"]
    }],
  "usr2var": [{
      "usr": 664645370489364250,
      "detailed_name": "Foo<B> b",
      "qual_name_offset": 7,
      "short_name": "b",
      "spell": "10:8-10:9|10:1-10:9|2|-1",
      "type": 159242453973086545,
      "kind": 13,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 8659751582206604765,
      "detailed_name": "Foo<A>::Inner a",
      "qual_name_offset": 14,
      "short_name": "a",
      "spell": "9:15-9:16|9:1-9:16|2|-1",
      "type": 3320579157636203467,
      "kind": 13,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
