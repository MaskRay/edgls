namespace hello {
class Foo {
  void foo();
};

void Foo::foo() {}
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 5795148097240184968,
      "detailed_name": "void hello::Foo::foo()",
      "qual_name_offset": 5,
      "short_name": "foo",
      "spell": "6:11-6:14|6:1-6:19|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 6,
      "parent_kind": 5,
      "storage": 0,
      "declarations": ["3:8-3:11|3:3-3:13|1|-1"],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
      "usr": 3615952160999596860,
      "detailed_name": "class hello::Foo {}",
      "qual_name_offset": 6,
      "short_name": "Foo",
      "spell": "2:7-2:10|2:1-4:2|2|-1",
      "bases": [],
      "funcs": [5795148097240184968],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 3,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["6:6-6:9|4|-1"]
    }, {
      "usr": 9568013184638781012,
      "detailed_name": "namespace hello {}",
      "qual_name_offset": 10,
      "short_name": "hello",
      "bases": [],
      "funcs": [],
      "types": [3615952160999596860],
      "vars": [],
      "alias_of": 0,
      "kind": 3,
      "parent_kind": 0,
      "declarations": ["1:11-1:16|1:1-7:2|1|-1"],
      "derived": [],
      "instances": [],
      "uses": []
    }],
  "usr2var": []
}
*/
