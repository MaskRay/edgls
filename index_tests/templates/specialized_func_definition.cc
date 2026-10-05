template<class T>
class Template {
  void Foo();
};

template<class T>
void Template<T>::Foo() {}

template<>
void Template<void>::Foo() {}

/*
// TODO: usage information on Template is bad.
// TODO: Foo() should have multiple definitions.

EXTRA_FLAGS:
-fms-compatibility
-fdelayed-template-parsing

OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 4247530818266671148,
      "detailed_name": "void Template<T>::Foo()",
      "qual_name_offset": 5,
      "short_name": "Foo",
      "spell": "10:22-10:25|10:1-10:30|2|-1",
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
      "usr": 1717983744634636841,
      "detailed_name": "class Template {}",
      "qual_name_offset": 6,
      "short_name": "Template",
      "spell": "2:7-2:15|2:1-4:2|2|-1",
      "bases": [],
      "funcs": [4247530818266671148],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["7:6-7:14|4|-1", "10:6-10:14|4|-1"]
    }, {
      "usr": 2850581126302235391,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "1:16-1:17|1:10-4:2|2|-1",
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
      "usr": 14037264256424742270,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "6:16-6:17|6:10-7:27|2|-1",
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
      "uses": ["7:15-7:16|4|-1"]
    }],
  "usr2var": []
}
*/
