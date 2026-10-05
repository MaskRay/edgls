struct Foo;

void foo(Foo* f, Foo*);
void foo(Foo* f, Foo*) {}

/*
// TODO: No interesting usage on prototype. But maybe that's ok!
// TODO: We should have the same variable declared for both prototype and
//       declaration. So it should have a usage marker on both. Then we could
//       rename parameters!

OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 11259702582602435674,
      "detailed_name": "void foo(Foo *f, Foo *)",
      "qual_name_offset": 5,
      "short_name": "foo",
      "spell": "4:6-4:9|4:1-4:26|2|-1",
      "bases": [],
      "vars": [16434186526842193651, 8730244993862947045],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": ["3:6-3:9|3:1-3:23|1|-1"],
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
      "instances": [16434186526842193651, 8730244993862947045],
      "uses": ["3:10-3:13|4|-1", "3:18-3:21|4|-1", "4:10-4:13|4|-1", "4:18-4:21|4|-1"]
    }],
  "usr2var": [{
      "usr": 8730244993862947045,
      "detailed_name": "Foo *f",
      "qual_name_offset": 5,
      "short_name": "f",
      "spell": "4:15-4:16|4:10-4:16|2|-1",
      "type": 11569259296725656241,
      "kind": 253,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 16434186526842193651,
      "detailed_name": "Foo *f",
      "qual_name_offset": 5,
      "short_name": "f",
      "spell": "3:15-3:16|3:10-3:16|2|-1",
      "type": 11569259296725656241,
      "kind": 253,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
