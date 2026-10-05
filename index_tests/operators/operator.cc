class Foo {
  void operator()(int) { }
  void operator()(bool);
  int operator()(int a, int b);
};

Foo &operator += (const Foo&, const int&);

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 10770519336392752005,
      "detailed_name": "void Foo::operator()(bool)",
      "qual_name_offset": 5,
      "short_name": "operator()",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 6,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["3:8-3:18|3:3-3:24|1|-1"],
      "derived": [],
      "uses": []
    }, {
      "usr": 14947430458250883580,
      "detailed_name": "void Foo::operator()(int)",
      "qual_name_offset": 5,
      "short_name": "operator()",
      "spell": "2:8-2:18|2:3-2:27|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 6,
      "parent_kind": 5,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 15810486314104033638,
      "detailed_name": "int Foo::operator()(int a, int b)",
      "qual_name_offset": 4,
      "short_name": "operator()",
      "bases": [],
      "vars": [7665651083513917524],
      "callees": [],
      "kind": 6,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["4:7-4:17|4:3-4:31|1|-1"],
      "derived": [],
      "uses": []
    }, {
      "usr": 16880648911976477006,
      "detailed_name": "Foo &operator+=(const Foo &, const int &)",
      "qual_name_offset": 5,
      "short_name": "operator+=",
      "bases": [],
      "vars": [10142981079666690152],
      "callees": [],
      "kind": 12,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["7:6-7:17|7:1-7:42|1|-1"],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
      "usr": 11569259296725656241,
      "detailed_name": "class Foo {}",
      "qual_name_offset": 6,
      "short_name": "Foo",
      "spell": "1:7-1:10|1:1-5:2|2|-1",
      "bases": [],
      "funcs": [14947430458250883580, 10770519336392752005, 15810486314104033638],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["7:1-7:4|4|-1", "7:25-7:28|4|-1"]
    }],
  "usr2var": [{
      "usr": 7665651083513917524,
      "detailed_name": "int a",
      "qual_name_offset": 4,
      "short_name": "a",
      "spell": "4:22-4:23|4:18-4:23|2|-1",
      "type": 0,
      "kind": 253,
      "parent_kind": 6,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 10142981079666690152,
      "detailed_name": "int b",
      "qual_name_offset": 4,
      "short_name": "b",
      "spell": "4:29-4:30|4:25-4:30|2|-1",
      "type": 0,
      "kind": 253,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
