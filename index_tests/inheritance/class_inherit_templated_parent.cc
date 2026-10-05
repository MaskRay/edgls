template<unsigned int C>
class Base1 {};

template<typename C>
class Base2 {};

template<unsigned int T>
class Derived1 : Base1<T> {};

template<typename T>
class Derived2 : Base2<T> {};

class Derived : Base1<3>, Base2<Derived>, Derived1<4>, Derived2<Derived> {};

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [],
  "usr2type": [{
      "usr": 3259294215276162192,
      "detailed_name": "class Base2 {}",
      "qual_name_offset": 6,
      "short_name": "Base2",
      "spell": "5:7-5:12|5:1-5:15|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [14447834637726212056, 9739895618756211179],
      "instances": [],
      "uses": ["11:18-11:23|4|-1", "13:27-13:32|4|-1"]
    }, {
      "usr": 6234793527839794371,
      "detailed_name": "class Base1 {}",
      "qual_name_offset": 6,
      "short_name": "Base1",
      "spell": "2:7-2:12|2:1-2:15|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [11323094752536999485, 9739895618756211179],
      "instances": [],
      "uses": ["8:18-8:23|4|-1", "13:17-13:22|4|-1"]
    }, {
      "usr": 9739895618756211179,
      "detailed_name": "class Derived : Base1<3U>, Base2<Derived>, Derived1<4U>, Derived2<Derived> {}",
      "qual_name_offset": 6,
      "short_name": "Derived",
      "spell": "13:7-13:14|13:1-13:76|2|-1",
      "bases": [6234793527839794371, 3259294215276162192, 11323094752536999485, 14447834637726212056],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["13:33-13:40|4|-1", "13:65-13:72|4|-1"]
    }, {
      "usr": 10577236311573157690,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "10:19-10:20|10:10-11:29|2|-1",
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
      "uses": ["11:24-11:25|4|-1"]
    }, {
      "usr": 11323094752536999485,
      "detailed_name": "class Derived1 : Base1<T> {}",
      "qual_name_offset": 6,
      "short_name": "Derived1",
      "spell": "8:7-8:15|8:1-8:29|2|-1",
      "bases": [6234793527839794371],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [9739895618756211179],
      "instances": [],
      "uses": ["13:43-13:51|4|-1"]
    }, {
      "usr": 14447834637726212056,
      "detailed_name": "class Derived2 : Base2<T> {}",
      "qual_name_offset": 6,
      "short_name": "Derived2",
      "spell": "11:7-11:15|11:1-11:29|2|-1",
      "bases": [3259294215276162192],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [9739895618756211179],
      "instances": [],
      "uses": ["13:56-13:64|4|-1"]
    }, {
      "usr": 14802205706358365897,
      "detailed_name": "C",
      "qual_name_offset": 0,
      "short_name": "C",
      "spell": "4:19-4:20|4:10-5:15|2|-1",
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
      "usr": 7050866332841925231,
      "detailed_name": "unsigned int T",
      "qual_name_offset": 13,
      "short_name": "T",
      "spell": "7:23-7:24|7:10-8:29|2|-1",
      "type": 0,
      "kind": 253,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": ["8:24-8:25|4|-1"]
    }, {
      "usr": 17918624099939626139,
      "detailed_name": "unsigned int C",
      "qual_name_offset": 13,
      "short_name": "C",
      "spell": "1:23-1:24|1:10-2:15|2|-1",
      "type": 0,
      "kind": 253,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
