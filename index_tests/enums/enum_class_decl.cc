typedef unsigned char uint8_t;
enum class Foo : uint8_t {
  A,
  B = 20
};

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [],
  "usr2type": [{
      "usr": 11569259296725656241,
      "detailed_name": "enum class Foo : uint8_t {}",
      "qual_name_offset": 11,
      "short_name": "Foo",
      "spell": "2:12-2:15|2:1-5:2|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [{
          "L": 14764555744278245583,
          "R": -1
        }, {
          "L": 7566877418593801577,
          "R": -1
        }],
      "alias_of": 0,
      "kind": 10,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": []
    }, {
      "usr": 16598673977360118010,
      "detailed_name": "typedef unsigned char uint8_t",
      "qual_name_offset": 22,
      "short_name": "uint8_t",
      "spell": "1:23-1:30|1:1-1:30|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 252,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["2:18-2:25|4|-1"]
    }],
  "usr2var": [{
      "usr": 7566877418593801577,
      "detailed_name": "Foo::B = 20U",
      "qual_name_offset": 0,
      "short_name": "B",
      "spell": "4:3-4:4|4:3-4:9|2|-1",
      "type": 11569259296725656241,
      "kind": 22,
      "parent_kind": 10,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 14764555744278245583,
      "detailed_name": "Foo::A",
      "qual_name_offset": 0,
      "short_name": "A",
      "spell": "3:3-3:4|3:3-3:4|2|-1",
      "type": 11569259296725656241,
      "kind": 22,
      "parent_kind": 10,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
