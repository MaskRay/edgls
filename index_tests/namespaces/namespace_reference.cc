namespace ns {
  int Foo;
  void Accept(int a) {}
}

void Runner() {
  ns::Accept(ns::Foo);
  using namespace ns;
  Accept(Foo);
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 10254232070186954932,
      "detailed_name": "void Runner()",
      "qual_name_offset": 5,
      "short_name": "Runner",
      "spell": "6:6-6:12|6:1-10:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": ["7:7-7:13|15940955418707670557|3|36", "9:3-9:9|15940955418707670557|3|36"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 15940955418707670557,
      "detailed_name": "void ns::Accept(int a)",
      "qual_name_offset": 5,
      "short_name": "Accept",
      "spell": "3:8-3:14|3:3-3:24|2|-1",
      "bases": [],
      "vars": [8206689863521543215],
      "callees": [],
      "kind": 12,
      "parent_kind": 3,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["7:7-7:13|36|-1", "9:3-9:9|36|-1"]
    }],
  "usr2type": [{
      "usr": 6736610825345693956,
      "detailed_name": "namespace ns {}",
      "qual_name_offset": 10,
      "short_name": "ns",
      "bases": [],
      "funcs": [15940955418707670557],
      "types": [],
      "vars": [{
          "L": 3697934033890140918,
          "R": -1
        }],
      "alias_of": 0,
      "kind": 3,
      "parent_kind": 0,
      "declarations": ["1:11-1:13|1:1-4:2|1|-1"],
      "derived": [],
      "instances": [],
      "uses": ["7:3-7:5|4|-1", "7:14-7:16|4|-1", "8:19-8:21|4|-1"]
    }],
  "usr2var": [{
      "usr": 3697934033890140918,
      "detailed_name": "int ns::Foo",
      "qual_name_offset": 4,
      "short_name": "Foo",
      "spell": "2:7-2:10|2:3-2:10|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 3,
      "storage": 0,
      "declarations": [],
      "uses": ["7:18-7:21|12|-1", "9:10-9:13|12|-1"]
    }, {
      "usr": 8206689863521543215,
      "detailed_name": "int a",
      "qual_name_offset": 4,
      "short_name": "a",
      "spell": "3:19-3:20|3:15-3:20|2|-1",
      "type": 0,
      "kind": 253,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
