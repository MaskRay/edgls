enum A {};
enum B {};

template<typename T>
T var = T();

A a = var<A>;
B b = var<B>;

// NOTE: libclang before 4.0 doesn't expose template usage on |var|.

#if false
EnumDecl A
EnumDecl B
UnexposedDecl var
VarDecl a
  UnexposedExpr var
    UnexposedExpr var
      DeclRefExpr var
        TypeRef enum A
UnexposedDecl var
VarDecl b
  UnexposedExpr var
    UnexposedExpr var
      DeclRefExpr var
        TypeRef enum B
UnexposedDecl var
#endif

/*
EXTRA_FLAGS:
-std=c++14

OUTPUT:
{
  "includes": [],
  "skipped_ranges": ["12:1-28:7"],
  "usr2func": [],
  "usr2type": [{
      "usr": 1728116131332111702,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "4:19-4:20|4:10-5:12|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 26,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [12827870401035390093],
      "uses": ["5:1-5:2|4|-1", "5:9-5:10|4|-1"]
    }, {
      "usr": 7410003400252181498,
      "detailed_name": "enum B {}",
      "qual_name_offset": 5,
      "short_name": "B",
      "spell": "2:6-2:7|2:1-2:10|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 10,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [664645370489364250],
      "uses": ["8:1-8:2|4|-1", "8:11-8:12|4|-1"]
    }, {
      "usr": 17635323982382475078,
      "detailed_name": "enum A {}",
      "qual_name_offset": 5,
      "short_name": "A",
      "spell": "1:6-1:7|1:1-1:10|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 10,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [8659751582206604765],
      "uses": ["7:1-7:2|4|-1", "7:11-7:12|4|-1"]
    }],
  "usr2var": [{
      "usr": 664645370489364250,
      "detailed_name": "B b",
      "qual_name_offset": 2,
      "short_name": "b",
      "hover": "B b = var<B>",
      "spell": "8:3-8:4|8:1-8:13|2|-1",
      "type": 7410003400252181498,
      "kind": 13,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 8659751582206604765,
      "detailed_name": "A a",
      "qual_name_offset": 2,
      "short_name": "a",
      "hover": "A a = var<A>",
      "spell": "7:3-7:4|7:1-7:13|2|-1",
      "type": 17635323982382475078,
      "kind": 13,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 12827870401035390093,
      "detailed_name": "T var<T>",
      "qual_name_offset": 2,
      "short_name": "var",
      "spell": "5:3-5:6|5:1-5:6|2|-1",
      "type": 1728116131332111702,
      "kind": 13,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": ["7:7-7:10|12|-1", "8:7-8:10|12|-1"]
    }]
}
*/
