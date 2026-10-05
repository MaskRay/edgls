class IFoo {
  virtual void foo() = 0;
};

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 6138964699182365317,
      "detailed_name": "virtual void IFoo::foo() = 0",
      "qual_name_offset": 13,
      "short_name": "foo",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 6,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["2:16-2:19|2:3-2:25|1|-1"],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
      "usr": 12645392084887642845,
      "detailed_name": "class IFoo {}",
      "qual_name_offset": 6,
      "short_name": "IFoo",
      "spell": "1:7-1:11|1:1-3:2|2|-1",
      "bases": [],
      "funcs": [6138964699182365317],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": []
    }],
  "usr2var": []
}
*/
