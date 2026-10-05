namespace {
void foo();
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 12363175341728128669,
      "detailed_name": "void (anon ns)::foo()",
      "qual_name_offset": 5,
      "short_name": "foo",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["2:6-2:9|2:1-2:11|1|-1"],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
      "usr": 10886505178030160540,
      "detailed_name": "anon ns",
      "qual_name_offset": 0,
      "short_name": "anon ns",
      "bases": [],
      "funcs": [12363175341728128669],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 3,
      "parent_kind": 0,
      "declarations": ["1:11-1:12|1:11-3:2|1|-1"],
      "derived": [],
      "instances": [],
      "uses": []
    }],
  "usr2var": []
}
*/
