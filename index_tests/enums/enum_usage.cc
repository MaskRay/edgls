enum class Foo {
  A,
  B = 20
};

Foo x = Foo::A;

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [],
  "usr2type": [{
      "usr": 11569259296725656241,
      "detailed_name": "enum class Foo {}",
      "qual_name_offset": 11,
      "short_name": "Foo",
      "spell": "1:12-1:15|1:1-4:2|2|-1",
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
      "instances": [6188258924115695395],
      "uses": ["6:1-6:4|4|-1", "6:9-6:12|4|-1"]
    }],
  "usr2var": [{
      "usr": 6188258924115695395,
      "detailed_name": "Foo x",
      "qual_name_offset": 4,
      "short_name": "x",
      "hover": "Foo x = Foo::A",
      "spell": "6:5-6:6|6:1-6:15|2|-1",
      "type": 11569259296725656241,
      "kind": 13,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 7566877418593801577,
      "detailed_name": "Foo::B = 20",
      "qual_name_offset": 0,
      "short_name": "B",
      "spell": "3:3-3:4|3:3-3:9|2|-1",
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
      "spell": "2:3-2:4|2:3-2:4|2|-1",
      "type": 11569259296725656241,
      "kind": 22,
      "parent_kind": 10,
      "storage": 0,
      "declarations": [],
      "uses": ["6:14-6:15|4|-1"]
    }]
}
*/
