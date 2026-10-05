template<typename T>
class unique_ptr {};

struct S;

static unique_ptr<S> foo;

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [],
  "usr2type": [{
      "usr": 3795401214890978402,
      "detailed_name": "struct S",
      "qual_name_offset": 7,
      "short_name": "S",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 0,
      "declarations": ["4:8-4:9|4:1-4:9|1|-1"],
      "derived": [],
      "instances": [],
      "uses": ["6:19-6:20|4|-1"]
    }, {
      "usr": 10529446884348658338,
      "detailed_name": "class unique_ptr {}",
      "qual_name_offset": 6,
      "short_name": "unique_ptr",
      "spell": "2:7-2:17|2:1-2:20|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [2382246765042007543],
      "uses": ["6:8-6:18|4|-1"]
    }, {
      "usr": 11829286778893761202,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "1:19-1:20|1:10-2:20|2|-1",
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
      "usr": 2382246765042007543,
      "detailed_name": "static unique_ptr<S> foo",
      "qual_name_offset": 21,
      "short_name": "foo",
      "spell": "6:22-6:25|6:1-6:25|2|-1",
      "type": 10529446884348658338,
      "kind": 13,
      "parent_kind": 1,
      "storage": 2,
      "declarations": [],
      "uses": []
    }]
}
*/
