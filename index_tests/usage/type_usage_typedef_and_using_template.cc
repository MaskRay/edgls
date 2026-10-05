template<typename T>
struct Foo;

using Foo1 = Foo<int>;
typedef Foo<Foo1> Foo2;

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [],
  "usr2type": [{
      "usr": 159242453973086545,
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
      "declarations": ["2:8-2:11|2:1-2:11|1|-1"],
      "derived": [],
      "instances": [],
      "uses": ["4:14-4:17|4|-1", "5:9-5:12|4|-1"]
    }, {
      "usr": 10874559570462461057,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "1:19-1:20|1:10-2:11|2|-1",
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
    }, {
      "usr": 14893556366640030606,
      "detailed_name": "typedef Foo<Foo1> Foo2",
      "qual_name_offset": 18,
      "short_name": "Foo2",
      "spell": "5:19-5:23|5:1-5:23|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 159242453973086545,
      "kind": 252,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": []
    }, {
      "usr": 16716315184296843987,
      "detailed_name": "using Foo1 = Foo<int>",
      "qual_name_offset": 6,
      "short_name": "Foo1",
      "spell": "4:7-4:11|4:1-4:22|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 159242453973086545,
      "kind": 252,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["5:13-5:17|4|-1"]
    }],
  "usr2var": []
}
*/
