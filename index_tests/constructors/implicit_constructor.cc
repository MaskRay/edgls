struct Type {
  Type() {}
};

void Make() {
  Type foo0;
  auto foo1 = Type();
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 13623298002609801415,
      "detailed_name": "void Make()",
      "qual_name_offset": 5,
      "short_name": "Make",
      "spell": "5:6-5:10|5:1-8:2|2|-1",
      "bases": [],
      "vars": [5976561531223218575, 11047734783445260224],
      "callees": ["6:8-6:12|18022452545510231053|3|292", "7:15-7:19|18022452545510231053|3|292"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 18022452545510231053,
      "detailed_name": "Type::Type()",
      "qual_name_offset": 0,
      "short_name": "Type",
      "spell": "2:3-2:7|2:3-2:12|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 9,
      "parent_kind": 23,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["6:8-6:12|292|-1", "7:15-7:19|292|-1"]
    }],
  "usr2type": [{
      "usr": 15323574594624129250,
      "detailed_name": "struct Type {}",
      "qual_name_offset": 7,
      "short_name": "Type",
      "spell": "1:8-1:12|1:1-3:2|2|-1",
      "bases": [],
      "funcs": [18022452545510231053],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [5976561531223218575, 11047734783445260224],
      "uses": ["2:3-2:7|4|-1", "6:3-6:7|4|-1", "7:15-7:19|4|-1"]
    }],
  "usr2var": [{
      "usr": 5976561531223218575,
      "detailed_name": "Type foo0",
      "qual_name_offset": 5,
      "short_name": "foo0",
      "spell": "6:8-6:12|6:3-6:12|2|-1",
      "type": 15323574594624129250,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 11047734783445260224,
      "detailed_name": "Type foo1",
      "qual_name_offset": 5,
      "short_name": "foo1",
      "hover": "Type foo1 = Type()",
      "spell": "7:8-7:12|7:3-7:21|2|-1",
      "type": 15323574594624129250,
      "kind": 13,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
