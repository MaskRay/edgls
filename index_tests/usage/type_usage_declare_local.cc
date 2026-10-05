struct ForwardType;
struct ImplementedType {};

void Foo() {
  ForwardType* a;
  ImplementedType b;
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 2213156063291502229,
      "detailed_name": "void Foo()",
      "qual_name_offset": 5,
      "short_name": "Foo",
      "spell": "4:6-4:9|4:1-7:2|2|-1",
      "bases": [],
      "vars": [1467773660927809121, 2051501308188942814],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
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
      "instances": [1467773660927809121],
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
      "instances": [2051501308188942814],
      "uses": ["6:3-6:18|4|-1"]
    }],
  "usr2var": [{
      "usr": 1467773660927809121,
      "detailed_name": "ForwardType *a",
      "qual_name_offset": 13,
      "short_name": "a",
      "spell": "5:16-5:17|5:3-5:17|2|-1",
      "type": 16819242541367499852,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 2051501308188942814,
      "detailed_name": "ImplementedType b",
      "qual_name_offset": 16,
      "short_name": "b",
      "spell": "6:19-6:20|6:3-6:20|2|-1",
      "type": 17410777737764246177,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
