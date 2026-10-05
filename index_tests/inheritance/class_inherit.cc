class Parent {};
class Derived : public Parent {};

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [],
  "usr2type": [{
      "usr": 7681530285317092345,
      "detailed_name": "class Parent {}",
      "qual_name_offset": 6,
      "short_name": "Parent",
      "spell": "1:7-1:13|1:1-1:16|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [9739895618756211179],
      "instances": [],
      "uses": ["2:24-2:30|4|-1"]
    }, {
      "usr": 9739895618756211179,
      "detailed_name": "class Derived : Parent {}",
      "qual_name_offset": 6,
      "short_name": "Derived",
      "spell": "2:7-2:14|2:1-2:33|2|-1",
      "bases": [7681530285317092345],
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
    }],
  "usr2var": []
}
*/
