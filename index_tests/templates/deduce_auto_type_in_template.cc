struct Foo {};
struct Bar {};
using Alias = Foo;

// Only instantiation: x is Foo.
template <typename T> void one(T t) {
  auto x = t;
}
// Two instantiations: y stays auto.
template <typename T> void two(T t) {
  auto y = t;
}

// Instantiated early (deduced return type) and again later: w stays auto.
namespace N {
template <typename T> auto three(T t) {
  auto w = t;
  return w;
}
Foo foo = three(Foo());
} // namespace N
Bar bar = N::three(Bar());

void f() {
  one(Foo());
  two(Foo());
  two(Bar());
  auto z = Alias();
}

// Locals and parameters of the instantiations are not indexed separately.

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 3583188547440855904,
      "detailed_name": "void two(T t)",
      "qual_name_offset": 5,
      "short_name": "two",
      "spell": "10:28-10:31|10:23-12:2|2|-1",
      "comments": "Two instantiations: y stays auto.",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["26:3-26:6|36|-1", "27:3-27:6|36|-1"]
    }, {
      "usr": 8041731272366646527,
      "detailed_name": "void f()",
      "qual_name_offset": 5,
      "short_name": "f",
      "spell": "24:6-24:7|24:1-29:2|2|-1",
      "bases": [],
      "vars": [13848867188523293349],
      "callees": ["25:3-25:6|13918275750237058045|3|36", "26:3-26:6|3583188547440855904|3|36", "27:3-27:6|3583188547440855904|3|36"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 11341509151713178180,
      "detailed_name": "auto N::three(T t)",
      "qual_name_offset": 5,
      "short_name": "three",
      "spell": "16:28-16:33|16:23-19:2|2|-1",
      "bases": [],
      "vars": [13933617907038910613, 7334482941749622570],
      "callees": [],
      "kind": 12,
      "parent_kind": 3,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["20:11-20:16|36|-1", "22:14-22:19|36|-1"]
    }, {
      "usr": 13918275750237058045,
      "detailed_name": "void one(T t)",
      "qual_name_offset": 5,
      "short_name": "one",
      "spell": "6:28-6:31|6:23-8:2|2|-1",
      "comments": "Only instantiation: x is Foo.",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["25:3-25:6|36|-1"]
    }],
  "usr2type": [{
      "usr": 5615179467931063350,
      "detailed_name": "struct Bar {}",
      "qual_name_offset": 7,
      "short_name": "Bar",
      "spell": "2:8-2:11|2:1-2:14|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [14761426726179313752, 13933617907038910613, 7334482941749622570],
      "uses": ["22:1-22:4|4|-1", "22:20-22:23|4|-1", "27:7-27:10|4|-1"]
    }, {
      "usr": 6286095134707920647,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "16:20-16:21|16:11-19:2|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 26,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [13933617907038910613],
      "uses": ["16:34-16:35|4|-1"]
    }, {
      "usr": 8322825658053237014,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "comments": "Two instantiations: y stays auto.",
      "spell": "10:20-10:21|10:11-12:2|2|-1",
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
      "uses": ["10:32-10:33|4|-1"]
    }, {
      "usr": 9899170766240741024,
      "detailed_name": "namespace N {}",
      "qual_name_offset": 10,
      "short_name": "N",
      "comments": "Instantiated early (deduced return type) and again later: w stays auto.",
      "bases": [],
      "funcs": [11341509151713178180],
      "types": [],
      "vars": [{
          "L": 8979726968320768441,
          "R": -1
        }],
      "alias_of": 0,
      "kind": 3,
      "parent_kind": 0,
      "declarations": ["15:11-15:12|15:1-21:2|1|-1"],
      "derived": [],
      "instances": [],
      "uses": ["22:11-22:12|4|-1"]
    }, {
      "usr": 11569259296725656241,
      "detailed_name": "struct Foo {}",
      "qual_name_offset": 7,
      "short_name": "Foo",
      "spell": "1:8-1:11|1:1-1:14|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [8979726968320768441, 13933617907038910613, 7334482941749622570],
      "uses": ["3:15-3:18|4|-1", "20:1-20:4|4|-1", "20:17-20:20|4|-1", "25:7-25:10|4|-1", "26:7-26:10|4|-1"]
    }, {
      "usr": 12141531263528593728,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "comments": "Only instantiation: x is Foo.",
      "spell": "6:20-6:21|6:11-8:2|2|-1",
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
      "uses": ["6:32-6:33|4|-1"]
    }, {
      "usr": 12739745473123755489,
      "detailed_name": "using Alias = Foo",
      "qual_name_offset": 6,
      "short_name": "Alias",
      "spell": "3:7-3:12|3:1-3:18|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 11569259296725656241,
      "kind": 252,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [13848867188523293349],
      "uses": ["28:12-28:17|4|-1"]
    }],
  "usr2var": [{
      "usr": 7334482941749622570,
      "detailed_name": "auto w",
      "qual_name_offset": 5,
      "short_name": "w",
      "hover": "auto w = t",
      "spell": "17:8-17:9|17:3-17:13|2|-1",
      "type": 5615179467931063350,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": ["18:10-18:11|4|-1"]
    }, {
      "usr": 8979726968320768441,
      "detailed_name": "Foo N::foo",
      "qual_name_offset": 4,
      "short_name": "foo",
      "hover": "Foo N::foo = three(Foo())",
      "spell": "20:5-20:8|20:1-20:23|2|-1",
      "type": 11569259296725656241,
      "kind": 13,
      "parent_kind": 3,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 13848867188523293349,
      "detailed_name": "Alias z",
      "qual_name_offset": 6,
      "short_name": "z",
      "hover": "Alias z = Alias()",
      "spell": "28:8-28:9|28:3-28:19|2|-1",
      "type": 12739745473123755489,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 13933617907038910613,
      "detailed_name": "T t",
      "qual_name_offset": 2,
      "short_name": "t",
      "spell": "16:36-16:37|16:34-16:37|2|-1",
      "type": 5615179467931063350,
      "kind": 253,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": ["17:12-17:13|4|-1"]
    }, {
      "usr": 14761426726179313752,
      "detailed_name": "Bar bar",
      "qual_name_offset": 4,
      "short_name": "bar",
      "hover": "Bar bar = N::three(Bar())",
      "spell": "22:5-22:8|22:1-22:26|2|-1",
      "type": 5615179467931063350,
      "kind": 13,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
