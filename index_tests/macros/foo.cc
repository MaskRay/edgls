#define A 5
#define DISALLOW(type) type(type&&) = delete;

struct Foo {
  DISALLOW(Foo);
};

int x = A;

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 6227857766576090988,
      "detailed_name": "Foo::Foo(Foo &&)",
      "qual_name_offset": 0,
      "short_name": "Foo",
      "spell": "5:3-5:11|5:3-5:11|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 9,
      "parent_kind": 23,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
      "usr": 11569259296725656241,
      "detailed_name": "struct Foo {}",
      "qual_name_offset": 7,
      "short_name": "Foo",
      "spell": "4:8-4:11|4:1-6:2|2|-1",
      "bases": [],
      "funcs": [6227857766576090988],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["5:3-5:11|4|-1"]
    }],
  "usr2var": [{
      "usr": 3209591113763758164,
      "detailed_name": "DISALLOW",
      "qual_name_offset": 0,
      "short_name": "DISALLOW",
      "hover": "#define DISALLOW(type) type(type&&) = delete;",
      "spell": "2:9-2:17|2:1-2:46|2|-1",
      "type": 0,
      "kind": 255,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": ["5:3-5:11|4|-1"]
    }, {
      "usr": 3255421164936759079,
      "detailed_name": "A",
      "qual_name_offset": 0,
      "short_name": "A",
      "hover": "#define A 5",
      "spell": "1:9-1:10|1:1-1:12|2|-1",
      "type": 0,
      "kind": 255,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": ["8:9-8:10|4|-1"]
    }, {
      "usr": 6188258924115695395,
      "detailed_name": "int x",
      "qual_name_offset": 4,
      "short_name": "x",
      "hover": "int x = A",
      "spell": "8:5-8:6|8:1-8:10|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
