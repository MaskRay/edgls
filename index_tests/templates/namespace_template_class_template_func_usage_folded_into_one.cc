namespace ns {
  template<typename T>
  struct Foo {
    template<typename R>
    static int foo() {
      return 3;
    }
  };

  int a = Foo<int>::foo<float>();
  int b = Foo<bool>::foo<double>();
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 3755425517499527338,
      "detailed_name": "static int ns::Foo<T>::foo()",
      "qual_name_offset": 11,
      "short_name": "foo",
      "spell": "5:16-5:19|5:5-7:6|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 254,
      "parent_kind": 23,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["10:21-10:24|36|-1", "11:22-11:25|36|-1"]
    }],
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
      "declarations": ["1:11-1:13|1:1-12:2|1|-1"],
      "derived": [],
      "instances": [],
      "uses": []
    }, {
      "usr": 7145393134778660483,
      "detailed_name": "struct ns::Foo {}",
      "qual_name_offset": 7,
      "short_name": "Foo",
      "spell": "3:10-3:13|3:3-8:4|2|-1",
      "bases": [],
      "funcs": [3755425517499527338],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 3,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["10:11-10:14|4|-1", "11:11-11:14|4|-1"]
    }, {
      "usr": 9044600004106426208,
      "detailed_name": "R",
      "qual_name_offset": 0,
      "short_name": "R",
      "spell": "4:23-4:24|4:14-7:6|2|-1",
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
      "usr": 15529199270083430389,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "2:21-2:22|2:12-8:4|2|-1",
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
      "detailed_name": "int ns::b",
      "qual_name_offset": 4,
      "short_name": "b",
      "hover": "int ns::b = Foo<bool>::foo<double>()",
      "spell": "11:7-11:8|11:3-11:35|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 3,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 16748637498875167592,
      "detailed_name": "int ns::a",
      "qual_name_offset": 4,
      "short_name": "a",
      "hover": "int ns::a = Foo<int>::foo<float>()",
      "spell": "10:7-10:8|10:3-10:33|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 3,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
