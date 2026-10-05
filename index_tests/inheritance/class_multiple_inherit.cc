class Root {};
class MiddleA : public Root {};
class MiddleB : public Root {};
class Derived : public MiddleA, public MiddleB {};

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [],
  "usr2type": [{
      "usr": 1422431814401995524,
      "detailed_name": "class Root {}",
      "qual_name_offset": 6,
      "short_name": "Root",
      "spell": "1:7-1:11|1:1-1:14|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [3091325548566871199, 14542204889346008023],
      "instances": [],
      "uses": ["2:24-2:28|4|-1", "3:24-3:28|4|-1"]
    }, {
      "usr": 3091325548566871199,
      "detailed_name": "class MiddleA : Root {}",
      "qual_name_offset": 6,
      "short_name": "MiddleA",
      "spell": "2:7-2:14|2:1-2:31|2|-1",
      "bases": [1422431814401995524],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [9739895618756211179],
      "instances": [],
      "uses": ["4:24-4:31|4|-1"]
    }, {
      "usr": 9739895618756211179,
      "detailed_name": "class Derived : MiddleA, MiddleB {}",
      "qual_name_offset": 6,
      "short_name": "Derived",
      "spell": "4:7-4:14|4:1-4:50|2|-1",
      "bases": [3091325548566871199, 14542204889346008023],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": []
    }, {
      "usr": 14542204889346008023,
      "detailed_name": "class MiddleB : Root {}",
      "qual_name_offset": 6,
      "short_name": "MiddleB",
      "spell": "3:7-3:14|3:1-3:31|2|-1",
      "bases": [1422431814401995524],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [9739895618756211179],
      "instances": [],
      "uses": ["4:40-4:47|4|-1"]
    }],
  "usr2var": []
}
*/
