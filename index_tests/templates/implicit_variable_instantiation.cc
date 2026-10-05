namespace ns {
  enum VarType {};

  template<typename _>
  struct Holder {
    static constexpr VarType static_var = (VarType)0x0;
  };

  template<typename _>
  const typename VarType Holder<_>::static_var;


  int Foo = Holder<int>::static_var;
  int Foo2 = Holder<int>::static_var;
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [],
  "usr2type": [{
      "usr": 714645689778112613,
      "detailed_name": "_",
      "qual_name_offset": 0,
      "short_name": "_",
      "spell": "9:21-9:22|9:12-10:47|2|-1",
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
      "uses": ["10:33-10:34|4|-1"]
    }, {
      "usr": 6736610825345693956,
      "detailed_name": "namespace ns {}",
      "qual_name_offset": 10,
      "short_name": "ns",
      "bases": [],
      "funcs": [],
      "types": [9093896321634663150, 11203248014380122928],
      "vars": [{
          "L": 3697934033890140918,
          "R": -1
        }, {
          "L": 871769824791614245,
          "R": -1
        }],
      "alias_of": 0,
      "kind": 3,
      "parent_kind": 0,
      "declarations": ["1:11-1:13|1:1-15:2|1|-1"],
      "derived": [],
      "instances": [],
      "uses": []
    }, {
      "usr": 9093896321634663150,
      "detailed_name": "enum ns::VarType {}",
      "qual_name_offset": 5,
      "short_name": "VarType",
      "spell": "2:8-2:15|2:3-2:18|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 10,
      "parent_kind": 3,
      "declarations": [],
      "derived": [],
      "instances": [11994346532281467775],
      "uses": ["6:22-6:29|4|-1", "6:44-6:51|4|-1"]
    }, {
      "usr": 11203248014380122928,
      "detailed_name": "struct ns::Holder {}",
      "qual_name_offset": 7,
      "short_name": "Holder",
      "spell": "5:10-5:16|5:3-7:4|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 3,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["10:26-10:32|4|-1", "13:13-13:19|4|-1", "14:14-14:20|4|-1"]
    }, {
      "usr": 16022971119637738475,
      "detailed_name": "_",
      "qual_name_offset": 0,
      "short_name": "_",
      "spell": "4:21-4:22|4:12-7:4|2|-1",
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
      "usr": 871769824791614245,
      "detailed_name": "int ns::Foo2",
      "qual_name_offset": 4,
      "short_name": "Foo2",
      "hover": "int ns::Foo2 = Holder<int>::static_var",
      "spell": "14:7-14:11|14:3-14:37|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 3,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 3697934033890140918,
      "detailed_name": "int ns::Foo",
      "qual_name_offset": 4,
      "short_name": "Foo",
      "hover": "int ns::Foo = Holder<int>::static_var",
      "spell": "13:7-13:10|13:3-13:36|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 3,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 11994346532281467775,
      "detailed_name": "static constexpr ns::VarType ns::Holder<_>::static_var",
      "qual_name_offset": 29,
      "short_name": "static_var",
      "hover": "static constexpr ns::VarType ns::Holder<_>::static_var = (VarType)0x0",
      "spell": "6:30-6:40|6:5-6:55|2|-1",
      "type": 9093896321634663150,
      "kind": 8,
      "parent_kind": 23,
      "storage": 0,
      "declarations": [],
      "uses": ["13:26-13:36|12|-1", "14:27-14:37|12|-1"]
    }]
}
*/
