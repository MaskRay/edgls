namespace foo {
    namespace bar {
         namespace baz {
             int qux = 42;
         }
    }
}

namespace fbz = foo::bar::baz;

void func() {
  int a = foo::bar::baz::qux;
  int b = fbz::qux;
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 6424962202363947293,
      "detailed_name": "void func()",
      "qual_name_offset": 5,
      "short_name": "func",
      "spell": "11:6-11:10|11:1-14:2|2|-1",
      "bases": [],
      "vars": [15204949827090961739, 2792580678849447192],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
      "usr": 2364553254444182587,
      "detailed_name": "namespace namespace fbz {}",
      "qual_name_offset": 10,
      "short_name": "nam",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 17833805335406229989,
      "kind": 3,
      "parent_kind": 0,
      "declarations": ["9:11-9:14|9:1-9:31|1|-1"],
      "derived": [],
      "instances": [],
      "uses": ["13:11-13:14|4|-1"]
    }, {
      "usr": 4591153257269892110,
      "detailed_name": "namespace foo::bar {}",
      "qual_name_offset": 10,
      "short_name": "bar",
      "bases": [10405152843845928970],
      "funcs": [],
      "types": [17833805335406229989],
      "vars": [],
      "alias_of": 0,
      "kind": 3,
      "parent_kind": 0,
      "declarations": ["2:15-2:18|2:5-6:6|1|-1"],
      "derived": [17833805335406229989],
      "instances": [],
      "uses": ["9:22-9:25|4|-1", "12:16-12:19|4|-1"]
    }, {
      "usr": 10405152843845928970,
      "detailed_name": "namespace foo {}",
      "qual_name_offset": 10,
      "short_name": "foo",
      "bases": [],
      "funcs": [],
      "types": [4591153257269892110],
      "vars": [],
      "alias_of": 0,
      "kind": 3,
      "parent_kind": 0,
      "declarations": ["1:11-1:14|1:1-7:2|1|-1"],
      "derived": [4591153257269892110],
      "instances": [],
      "uses": ["9:17-9:20|4|-1", "12:11-12:14|4|-1"]
    }, {
      "usr": 17833805335406229989,
      "detailed_name": "namespace foo::bar::baz {}",
      "qual_name_offset": 10,
      "short_name": "baz",
      "bases": [4591153257269892110],
      "funcs": [],
      "types": [],
      "vars": [{
          "L": 15718513160789729630,
          "R": -1
        }],
      "alias_of": 0,
      "kind": 3,
      "parent_kind": 0,
      "declarations": ["3:20-3:23|3:10-5:11|1|-1"],
      "derived": [],
      "instances": [],
      "uses": ["9:17-9:20|4|-1", "12:21-12:24|4|-1"]
    }],
  "usr2var": [{
      "usr": 2792580678849447192,
      "detailed_name": "int b",
      "qual_name_offset": 4,
      "short_name": "b",
      "hover": "int b = fbz::qux",
      "spell": "13:7-13:8|13:3-13:19|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 15204949827090961739,
      "detailed_name": "int a",
      "qual_name_offset": 4,
      "short_name": "a",
      "hover": "int a = foo::bar::baz::qux",
      "spell": "12:7-12:8|12:3-12:29|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 15718513160789729630,
      "detailed_name": "int foo::bar::baz::qux",
      "qual_name_offset": 4,
      "short_name": "qux",
      "hover": "int foo::bar::baz::qux = 42",
      "spell": "4:18-4:21|4:14-4:26|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 3,
      "storage": 0,
      "declarations": [],
      "uses": ["12:26-12:29|12|-1", "13:16-13:19|12|-1"]
    }]
}
*/
