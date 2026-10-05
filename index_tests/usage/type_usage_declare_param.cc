struct ForwardType;
struct ImplementedType {};

void foo(ForwardType* f, ImplementedType a) {}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 14832168444467123616,
      "detailed_name": "void foo(ForwardType *f, ImplementedType a)",
      "qual_name_offset": 5,
      "short_name": "foo",
      "spell": "4:6-4:9|4:1-4:47|2|-1",
      "bases": [],
      "vars": [3962455139803999520, 5623938557012010439],
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
      "instances": [3962455139803999520],
      "uses": ["4:10-4:21|4|-1"]
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
      "instances": [5623938557012010439],
      "uses": ["4:26-4:41|4|-1"]
    }],
  "usr2var": [{
      "usr": 3962455139803999520,
      "detailed_name": "ForwardType *f",
      "qual_name_offset": 13,
      "short_name": "f",
      "spell": "4:23-4:24|4:10-4:24|2|-1",
      "type": 16819242541367499852,
      "kind": 253,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 5623938557012010439,
      "detailed_name": "ImplementedType a",
      "qual_name_offset": 16,
      "short_name": "a",
      "spell": "4:42-4:43|4:26-4:43|2|-1",
      "type": 17410777737764246177,
      "kind": 253,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
