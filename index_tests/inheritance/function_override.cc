class Root {
  virtual void foo();
};
class Derived : public Root {
  void foo() override {}
};

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 7820007094232907752,
      "detailed_name": "void Derived::foo() override",
      "qual_name_offset": 5,
      "short_name": "foo",
      "spell": "5:8-5:11|5:3-5:25|2|-1",
      "bases": [12953178260744466222],
      "vars": [],
      "callees": [],
      "kind": 6,
      "parent_kind": 5,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 12953178260744466222,
      "detailed_name": "virtual void Root::foo()",
      "qual_name_offset": 13,
      "short_name": "foo",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 6,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["2:16-2:19|2:3-2:21|1|-1"],
      "derived": [7820007094232907752],
      "uses": []
    }],
  "usr2type": [{
      "usr": 1422431814401995524,
      "detailed_name": "class Root {}",
      "qual_name_offset": 6,
      "short_name": "Root",
      "spell": "1:7-1:11|1:1-3:2|2|-1",
      "bases": [],
      "funcs": [12953178260744466222],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [9739895618756211179],
      "instances": [],
      "uses": ["4:24-4:28|4|-1"]
    }, {
      "usr": 9739895618756211179,
      "detailed_name": "class Derived : Root {}",
      "qual_name_offset": 6,
      "short_name": "Derived",
      "spell": "4:7-4:14|4:1-6:2|2|-1",
      "bases": [1422431814401995524],
      "funcs": [7820007094232907752],
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
