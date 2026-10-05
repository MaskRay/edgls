template<typename T>
struct Foo {
  static int foo() {
    return 3;
  }
};

int a = Foo<int>::foo();
int b = Foo<bool>::foo();

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
      "spell": "3:14-3:17|3:3-5:4|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 254,
      "parent_kind": 23,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["8:19-8:22|36|-1", "9:20-9:23|36|-1"]
    }],
  "usr2type": [{
      "usr": 159242453973086545,
      "detailed_name": "struct Foo {}",
      "qual_name_offset": 7,
      "short_name": "Foo",
      "spell": "2:8-2:11|2:1-6:2|2|-1",
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
      "uses": ["8:9-8:12|4|-1", "9:9-9:12|4|-1"]
    }, {
      "usr": 687647803887773755,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "1:19-1:20|1:10-6:2|2|-1",
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
      "hover": "int b = Foo<bool>::foo()",
      "spell": "9:5-9:6|9:1-9:25|2|-1",
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
      "hover": "int a = Foo<int>::foo()",
      "spell": "8:5-8:6|8:1-8:24|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
