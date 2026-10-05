class Foo {
public:
  Foo() {}
  ~Foo() {};
};

void foo() {
  Foo f;
}

// TODO: Support destructors (notice how the dtor has no usages listed).
//        - check if variable is a pointer. if so, do *not* insert dtor
//        - check if variable is normal type. if so, insert dtor
//        - scan for statements that look like dtors in function def handler
//        - figure out some way to support w/ unique_ptrs?
/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 2059144113310522824,
      "detailed_name": "Foo::Foo()",
      "qual_name_offset": 0,
      "short_name": "Foo",
      "spell": "3:3-3:6|3:3-3:11|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 9,
      "parent_kind": 5,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["8:7-8:8|292|-1"]
    }, {
      "usr": 9608532274827022249,
      "detailed_name": "void foo()",
      "qual_name_offset": 5,
      "short_name": "foo",
      "spell": "7:6-7:9|7:1-9:2|2|-1",
      "bases": [],
      "vars": [12656081844634608715],
      "callees": ["8:7-8:8|2059144113310522824|3|292", "8:7-8:8|16654549582503144346|3|292"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 16654549582503144346,
      "detailed_name": "Foo::~Foo() noexcept",
      "qual_name_offset": 0,
      "short_name": "~Foo",
      "spell": "4:3-4:7|4:3-4:12|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 6,
      "parent_kind": 5,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["8:7-8:8|292|-1"]
    }],
  "usr2type": [{
      "usr": 11569259296725656241,
      "detailed_name": "class Foo {}",
      "qual_name_offset": 6,
      "short_name": "Foo",
      "spell": "1:7-1:10|1:1-5:2|2|-1",
      "bases": [],
      "funcs": [2059144113310522824, 16654549582503144346],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [12656081844634608715],
      "uses": ["3:3-3:6|4|-1", "4:4-4:7|4|-1", "8:3-8:6|4|-1"]
    }],
  "usr2var": [{
      "usr": 12656081844634608715,
      "detailed_name": "Foo f",
      "qual_name_offset": 4,
      "short_name": "f",
      "spell": "8:7-8:8|8:3-8:8|2|-1",
      "type": 11569259296725656241,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
