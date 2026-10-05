namespace ns {
  template<typename T>
  class Foo {};

  Foo<int> a;
  Foo<bool> b;
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [],
  "usr2type": [{
      "usr": 6736610825345693956,
      "detailed_name": "namespace ns {}",
      "qual_name_offset": 10,
      "short_name": "ns",
      "bases": [],
      "funcs": [],
      "types": [7145393134778660483],
      "vars": [{
          "L": 16748637498875167592,
          "R": -1
        }, {
          "L": 14896282477451295417,
          "R": -1
        }],
      "alias_of": 0,
      "kind": 3,
      "parent_kind": 0,
      "declarations": ["1:11-1:13|1:1-7:2|1|-1"],
      "derived": [],
      "instances": [],
      "uses": []
    }, {
      "usr": 7145393134778660483,
      "detailed_name": "class ns::Foo {}",
      "qual_name_offset": 6,
      "short_name": "Foo",
      "spell": "3:9-3:12|3:3-3:15|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 3,
      "declarations": [],
      "derived": [],
      "instances": [16748637498875167592, 14896282477451295417],
      "uses": ["5:3-5:6|4|-1", "6:3-6:6|4|-1"]
    }, {
      "usr": 10373702911830525509,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "2:21-2:22|2:12-3:15|2|-1",
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
  "usr2var": [{
      "usr": 14896282477451295417,
      "detailed_name": "ns::Foo<bool> ns::b",
      "qual_name_offset": 14,
      "short_name": "b",
      "spell": "6:13-6:14|6:3-6:14|2|-1",
      "type": 7145393134778660483,
      "kind": 13,
      "parent_kind": 3,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 16748637498875167592,
      "detailed_name": "ns::Foo<int> ns::a",
      "qual_name_offset": 13,
      "short_name": "a",
      "spell": "5:12-5:13|5:3-5:13|2|-1",
      "type": 7145393134778660483,
      "kind": 13,
      "parent_kind": 3,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
