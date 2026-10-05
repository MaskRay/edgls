struct Foo;

void foo(Foo* p0, Foo* p1) {}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 11259702582602435674,
      "detailed_name": "void foo(Foo *p0, Foo *p1)",
      "qual_name_offset": 5,
      "short_name": "foo",
      "spell": "3:6-3:9|3:1-3:30|2|-1",
      "bases": [],
      "vars": [6097553275954729153, 7524462698413538764],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
      "usr": 11569259296725656241,
      "detailed_name": "struct Foo",
      "qual_name_offset": 7,
      "short_name": "Foo",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 0,
      "declarations": ["1:8-1:11|1:1-1:11|1|-1"],
      "derived": [],
      "instances": [6097553275954729153, 7524462698413538764],
      "uses": ["3:10-3:13|4|-1", "3:19-3:22|4|-1"]
    }],
  "usr2var": [{
      "usr": 6097553275954729153,
      "detailed_name": "Foo *p0",
      "qual_name_offset": 5,
      "short_name": "p0",
      "spell": "3:15-3:17|3:10-3:17|2|-1",
      "type": 11569259296725656241,
      "kind": 253,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 7524462698413538764,
      "detailed_name": "Foo *p1",
      "qual_name_offset": 5,
      "short_name": "p1",
      "spell": "3:24-3:26|3:19-3:26|2|-1",
      "type": 11569259296725656241,
      "kind": 253,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
