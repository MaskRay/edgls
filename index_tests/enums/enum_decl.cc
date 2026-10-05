enum Foo {
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
      "detailed_name": "enum Foo {}",
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
    }],
  "usr2var": [{
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
