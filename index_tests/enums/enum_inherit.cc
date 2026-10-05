enum Foo : int {
  A,
  B = 20
};

typedef int int32_t;

enum class E : int32_t {
  E0,
  E20 = 20
};

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [],
  "usr2type": [{
      "usr": 7451110790946010822,
      "detailed_name": "typedef int int32_t",
      "qual_name_offset": 12,
      "short_name": "int32_t",
      "spell": "6:13-6:20|6:1-6:20|2|-1",
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
      "uses": ["8:16-8:23|4|-1"]
    }, {
      "usr": 11569259296725656241,
      "detailed_name": "enum Foo : int {}",
      "qual_name_offset": 5,
      "short_name": "Foo",
      "spell": "1:6-1:9|1:1-4:2|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [{
          "L": 6742167846147047236,
          "R": -1
        }, {
          "L": 12484003124849472749,
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
      "usr": 14628069262661708347,
      "detailed_name": "enum class E : int32_t {}",
      "qual_name_offset": 11,
      "short_name": "E",
      "spell": "8:12-8:13|8:1-11:2|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [{
          "L": 11584466693843459367,
          "R": -1
        }, {
          "L": 2110424254896352750,
          "R": -1
        }],
      "alias_of": 0,
      "kind": 10,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": []
    }],
  "usr2var": [{
      "usr": 2110424254896352750,
      "detailed_name": "E::E20 = 20",
      "qual_name_offset": 0,
      "short_name": "E20",
      "spell": "10:3-10:6|10:3-10:11|2|-1",
      "type": 14628069262661708347,
      "kind": 22,
      "parent_kind": 10,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 6742167846147047236,
      "detailed_name": "A",
      "qual_name_offset": 0,
      "short_name": "A",
      "spell": "2:3-2:4|2:3-2:4|2|-1",
      "type": 11569259296725656241,
      "kind": 22,
      "parent_kind": 10,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 11584466693843459367,
      "detailed_name": "E::E0",
      "qual_name_offset": 0,
      "short_name": "E0",
      "spell": "9:3-9:5|9:3-9:5|2|-1",
      "type": 14628069262661708347,
      "kind": 22,
      "parent_kind": 10,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 12484003124849472749,
      "detailed_name": "B = 20",
      "qual_name_offset": 0,
      "short_name": "B",
      "spell": "3:3-3:4|3:3-3:9|2|-1",
      "type": 11569259296725656241,
      "kind": 22,
      "parent_kind": 10,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
