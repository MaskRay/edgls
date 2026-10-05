template<typename T>
class unique_ptr {};

struct S {};

static unique_ptr<bool> f0;
static unique_ptr<S> f1;

unique_ptr<S>* return_type() {
  unique_ptr<S>* local;
  return nullptr;
}
/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 14034024216814806547,
      "detailed_name": "unique_ptr<S> *return_type()",
      "qual_name_offset": 15,
      "short_name": "return_type",
      "spell": "9:16-9:27|9:1-12:2|2|-1",
      "bases": [],
      "vars": [978703339104823742],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
      "usr": 3795401214890978402,
      "detailed_name": "struct S {}",
      "qual_name_offset": 7,
      "short_name": "S",
      "spell": "4:8-4:9|4:1-4:12|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["7:19-7:20|4|-1", "9:12-9:13|4|-1", "10:14-10:15|4|-1"]
    }, {
      "usr": 9569517496234360718,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "1:19-1:20|1:10-2:20|2|-1",
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
      "usr": 10529446884348658338,
      "detailed_name": "class unique_ptr {}",
      "qual_name_offset": 6,
      "short_name": "unique_ptr",
      "spell": "2:7-2:17|2:1-2:20|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [14509030670348447203, 14371257921803179393, 978703339104823742],
      "uses": ["6:8-6:18|4|-1", "7:8-7:18|4|-1", "9:1-9:11|4|-1", "10:3-10:13|4|-1"]
    }],
  "usr2var": [{
      "usr": 978703339104823742,
      "detailed_name": "unique_ptr<S> *local",
      "qual_name_offset": 15,
      "short_name": "local",
      "spell": "10:18-10:23|10:3-10:23|2|-1",
      "type": 10529446884348658338,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 14371257921803179393,
      "detailed_name": "static unique_ptr<S> f1",
      "qual_name_offset": 21,
      "short_name": "f1",
      "spell": "7:22-7:24|7:1-7:24|2|-1",
      "type": 10529446884348658338,
      "kind": 13,
      "parent_kind": 1,
      "storage": 2,
      "declarations": [],
      "uses": []
    }, {
      "usr": 14509030670348447203,
      "detailed_name": "static unique_ptr<bool> f0",
      "qual_name_offset": 24,
      "short_name": "f0",
      "spell": "6:25-6:27|6:1-6:27|2|-1",
      "type": 10529446884348658338,
      "kind": 13,
      "parent_kind": 1,
      "storage": 2,
      "declarations": [],
      "uses": []
    }]
}
*/
