
template <class T> struct type_trait {
  const static bool value = false;
};

template <> struct type_trait<int> {
  const static bool value = true;
};

template <class T>
concept Con1 = type_trait<T>::value;

constexpr int sizeFunc() { return 4; }

template <class T>
concept ConWithLogicalAnd = Con1<T> && sizeof(T) > sizeFunc();

namespace ns {
template <class T>
concept ConInNamespace = sizeof(T) > 4;
}

template <class T1, class T2>
concept ConTwoTemplateParams = ns::ConInNamespace<T1> && ConWithLogicalAnd<T2>;

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 12832109129898550044,
      "detailed_name": "constexpr int sizeFunc()",
      "qual_name_offset": 14,
      "short_name": "sizeFunc",
      "spell": "13:15-13:23|13:1-13:39|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["16:52-16:60|36|-1"]
    }],
  "usr2type": [{
      "usr": 1709575751259805968,
      "detailed_name": "T2",
      "qual_name_offset": 0,
      "short_name": "T2",
      "spell": "23:27-23:29|23:11-24:79|2|-1",
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
      "uses": ["24:76-24:78|4|-1"]
    }, {
      "usr": 1775997315955988664,
      "detailed_name": "concept ConWithLogicalAnd",
      "qual_name_offset": 8,
      "short_name": "ConWithLogicalAnd",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 11,
      "parent_kind": 0,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["24:58-24:75|4|-1"]
    }, {
      "usr": 2310493499000140627,
      "detailed_name": "concept Con1",
      "qual_name_offset": 8,
      "short_name": "Con1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 11,
      "parent_kind": 0,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["16:29-16:33|4|-1"]
    }, {
      "usr": 5436305835756198098,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "10:17-10:18|10:11-11:36|2|-1",
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
      "uses": ["11:27-11:28|4|-1"]
    }, {
      "usr": 6129146095402689688,
      "detailed_name": "T1",
      "qual_name_offset": 0,
      "short_name": "T1",
      "spell": "23:17-23:19|23:11-23:19|2|-1",
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
      "uses": ["24:51-24:53|4|-1"]
    }, {
      "usr": 6664572074299238908,
      "detailed_name": "struct type_trait {}",
      "qual_name_offset": 7,
      "short_name": "type_trait",
      "spell": "2:27-2:37|2:20-4:2|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [16976519585593699380],
      "instances": [],
      "uses": ["11:16-11:26|4|-1"]
    }, {
      "usr": 6736610825345693956,
      "detailed_name": "namespace ns {}",
      "qual_name_offset": 10,
      "short_name": "ns",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 3,
      "parent_kind": 0,
      "declarations": ["18:11-18:13|18:1-21:2|1|-1"],
      "derived": [],
      "instances": [],
      "uses": ["24:32-24:34|4|-1"]
    }, {
      "usr": 8648690851436968200,
      "detailed_name": "concept ns::ConInNamespace",
      "qual_name_offset": 8,
      "short_name": "ConInNamespace",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 11,
      "parent_kind": 0,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["24:36-24:50|4|-1"]
    }, {
      "usr": 9631733932579801650,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "19:17-19:18|19:11-20:39|2|-1",
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
      "uses": ["20:33-20:34|4|-1"]
    }, {
      "usr": 10001515945194388058,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "2:17-2:18|2:11-4:2|2|-1",
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
      "usr": 15338587424071383665,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "15:17-15:18|15:11-16:62|2|-1",
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
      "uses": ["16:34-16:35|4|-1", "16:47-16:48|4|-1"]
    }, {
      "usr": 16976519585593699380,
      "detailed_name": "struct type_trait<int> {}",
      "qual_name_offset": 7,
      "short_name": "type_trait",
      "spell": "6:20-6:30|6:13-8:2|2|-1",
      "bases": [6664572074299238908],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": []
    }],
  "usr2var": [{
      "usr": 4795791200396179937,
      "detailed_name": "static const bool type_trait<int>::value",
      "qual_name_offset": 18,
      "short_name": "value",
      "type": 0,
      "kind": 8,
      "parent_kind": 0,
      "storage": 1,
      "declarations": ["7:21-7:26|7:3-7:33|1|-1"],
      "uses": []
    }, {
      "usr": 8730826884427672591,
      "detailed_name": "static const bool type_trait<T>::value",
      "qual_name_offset": 18,
      "short_name": "value",
      "type": 0,
      "kind": 22,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["3:21-3:26|3:3-3:34|1|-1"],
      "uses": ["11:31-11:36|4|-1"]
    }]
}
*/
