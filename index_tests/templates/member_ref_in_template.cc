template <class T>
struct C {
  T x;
  void bar();
};

template <class T>
void foo() {
  C<T> d;
  d.x;  // spelling range is empty, use cursor extent for range
  d.bar();  // spelling range is empty, use cursor extent for range

  auto e = new C<T>;
  e->x;  // `x` seems not exposed by libclang
  e->bar();  // `bar` seems not exposed by libclang
}

/*
EXTRA_FLAGS:
-fms-extensions
-fms-compatibility
-fdelayed-template-parsing

OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 3438372280286921981,
      "detailed_name": "void C<T>::bar()",
      "qual_name_offset": 5,
      "short_name": "bar",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 6,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["4:8-4:11|4:3-4:13|1|-1"],
      "derived": [],
      "uses": []
    }, {
      "usr": 9608532274827022249,
      "detailed_name": "void foo()",
      "qual_name_offset": 5,
      "short_name": "foo",
      "spell": "8:6-8:9|8:1-16:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
      "usr": 424136711916680843,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "1:17-1:18|1:11-5:2|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 26,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [409094319899701248],
      "uses": ["3:3-3:4|4|-1"]
    }, {
      "usr": 4125333953201788602,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "7:17-7:18|7:11-16:2|2|-1",
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
      "usr": 14842913275446395422,
      "detailed_name": "struct C {}",
      "qual_name_offset": 7,
      "short_name": "C",
      "spell": "2:8-2:9|2:1-5:2|2|-1",
      "bases": [],
      "funcs": [3438372280286921981],
      "types": [],
      "vars": [{
          "L": 409094319899701248,
          "R": -1
        }],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": []
    }],
  "usr2var": [{
      "usr": 409094319899701248,
      "detailed_name": "T C<T>::x",
      "qual_name_offset": 2,
      "short_name": "x",
      "spell": "3:5-3:6|3:3-3:6|2|-1",
      "type": 424136711916680843,
      "kind": 8,
      "parent_kind": 23,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
