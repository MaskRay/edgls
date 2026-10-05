union vector3 {
  struct { float x, y, z; };
  float v[3];
};

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [],
  "usr2type": [{
      "usr": 9002503854244335173,
      "detailed_name": "union vector3 {}",
      "qual_name_offset": 6,
      "short_name": "vector3",
      "spell": "1:7-1:14|1:1-4:2|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [{
          "L": 6964591516709017249,
          "R": 0
        }],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": []
    }],
  "usr2var": [{
      "usr": 1611867660540184517,
      "detailed_name": "float vector3::(anon)::y",
      "qual_name_offset": 6,
      "short_name": "y",
      "spell": "2:21-2:22|2:12-2:22|2|-1",
      "type": 0,
      "kind": 8,
      "parent_kind": 23,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 5127578207004968863,
      "detailed_name": "float vector3::(anon)::z",
      "qual_name_offset": 6,
      "short_name": "z",
      "spell": "2:24-2:25|2:12-2:25|2|-1",
      "type": 0,
      "kind": 8,
      "parent_kind": 23,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 6964591516709017249,
      "detailed_name": "float vector3::v[3]",
      "qual_name_offset": 6,
      "short_name": "v",
      "spell": "3:9-3:10|3:3-3:13|2|-1",
      "type": 0,
      "kind": 8,
      "parent_kind": 5,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 8511302874629528486,
      "detailed_name": "float vector3::(anon)::x",
      "qual_name_offset": 6,
      "short_name": "x",
      "spell": "2:18-2:19|2:12-2:19|2|-1",
      "type": 0,
      "kind": 8,
      "parent_kind": 23,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
