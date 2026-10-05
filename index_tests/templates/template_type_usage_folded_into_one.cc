template<typename T>
class Foo {};

Foo<int> a;
Foo<bool> b;

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [],
  "usr2type": [{
      "usr": 159242453973086545,
      "detailed_name": "class Foo {}",
      "qual_name_offset": 6,
      "short_name": "Foo",
      "spell": "2:7-2:10|2:1-2:13|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [8659751582206604765, 664645370489364250],
      "uses": ["4:1-4:4|4|-1", "5:1-5:4|4|-1"]
    }, {
      "usr": 9945419726436390433,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "1:19-1:20|1:10-2:13|2|-1",
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
    }],
  "usr2var": [{
      "usr": 664645370489364250,
      "detailed_name": "Foo<bool> b",
      "qual_name_offset": 10,
      "short_name": "b",
      "spell": "5:11-5:12|5:1-5:12|2|-1",
      "type": 159242453973086545,
      "kind": 13,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 8659751582206604765,
      "detailed_name": "Foo<int> a",
      "qual_name_offset": 9,
      "short_name": "a",
      "spell": "4:10-4:11|4:1-4:11|2|-1",
      "type": 159242453973086545,
      "kind": 13,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
