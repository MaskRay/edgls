struct ForwardType;
struct ImplementedType {};

struct Foo {
  ForwardType* a;
  ImplementedType b;
};

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [],
  "usr2type": [{
      "usr": 11569259296725656241,
      "detailed_name": "struct Foo {}",
      "qual_name_offset": 7,
      "short_name": "Foo",
      "spell": "4:8-4:11|4:1-7:2|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [{
          "L": 9453358358722468265,
          "R": 0
        }, {
          "L": 13231693242476834285,
          "R": 64
        }],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": []
    }, {
      "usr": 16819242541367499852,
      "detailed_name": "struct ForwardType",
      "qual_name_offset": 7,
      "short_name": "ForwardType",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 0,
      "declarations": ["1:8-1:19|1:1-1:19|1|-1"],
      "derived": [],
      "instances": [9453358358722468265],
      "uses": ["5:3-5:14|4|-1"]
    }, {
      "usr": 17410777737764246177,
      "detailed_name": "struct ImplementedType {}",
      "qual_name_offset": 7,
      "short_name": "ImplementedType",
      "spell": "2:8-2:23|2:1-2:26|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [13231693242476834285],
      "uses": ["6:3-6:18|4|-1"]
    }],
  "usr2var": [{
      "usr": 9453358358722468265,
      "detailed_name": "ForwardType *Foo::a",
      "qual_name_offset": 13,
      "short_name": "a",
      "spell": "5:16-5:17|5:3-5:17|2|-1",
      "type": 16819242541367499852,
      "kind": 8,
      "parent_kind": 23,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 13231693242476834285,
      "detailed_name": "ImplementedType Foo::b",
      "qual_name_offset": 16,
      "short_name": "b",
      "spell": "6:19-6:20|6:3-6:20|2|-1",
      "type": 17410777737764246177,
      "kind": 8,
      "parent_kind": 23,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
