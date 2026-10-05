template<typename T>
struct Foo {
  template<typename R>
  static int foo() {
    return 3;
  }
};

int a = Foo<int>::foo<float>();
int b = Foo<bool>::foo<double>();

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 12659322300110838339,
      "detailed_name": "static int Foo<T>::foo()",
      "qual_name_offset": 11,
      "short_name": "foo",
      "spell": "4:14-4:17|4:3-6:4|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 254,
      "parent_kind": 23,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["9:19-9:22|36|-1", "10:20-10:23|36|-1"]
    }],
  "usr2type": [{
      "usr": 159242453973086545,
      "detailed_name": "struct Foo {}",
      "qual_name_offset": 7,
      "short_name": "Foo",
      "spell": "2:8-2:11|2:1-7:2|2|-1",
      "bases": [],
      "funcs": [12659322300110838339],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["9:9-9:12|4|-1", "10:9-10:12|4|-1"]
    }, {
      "usr": 660960233825245244,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "1:19-1:20|1:10-7:2|2|-1",
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
      "usr": 7364784582493031325,
      "detailed_name": "R",
      "qual_name_offset": 0,
      "short_name": "R",
      "spell": "3:21-3:22|3:12-6:4|2|-1",
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
      "usr": 664645370489364250,
      "detailed_name": "int b",
      "qual_name_offset": 4,
      "short_name": "b",
      "hover": "int b = Foo<bool>::foo<double>()",
      "spell": "10:5-10:6|10:1-10:33|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 8659751582206604765,
      "detailed_name": "int a",
      "qual_name_offset": 4,
      "short_name": "a",
      "hover": "int a = Foo<int>::foo<float>()",
      "spell": "9:5-9:6|9:1-9:31|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
