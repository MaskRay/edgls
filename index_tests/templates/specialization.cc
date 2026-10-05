template<class T>
class function;

template<typename T, typename... Args>
class function<T(Args...)> {};

function<void(int)> f;

template<typename T> class allocator;

template<typename T, typename Alloc = allocator<T> >
class vector {
  void clear();
};

template<typename T>
class vector<T*> {};

struct Z1 {};

template class vector<Z1>;

struct Z2 {};

template<>
class vector<Z2> {
  void clear();
};

vector<char> vc;
vector<int*> vip;
vector<Z1> vz1;
vector<Z2> vz2;

enum Enum {
  Enum0, Enum1
};
template <typename T, int I, Enum, int E>
void foo(T Value) {}

static const int kOnst = 7;
template <>
void foo<float, 9, Enum0, kOnst + 7>(float Value);

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 582647068104342030,
      "detailed_name": "void foo(T Value)",
      "qual_name_offset": 5,
      "short_name": "foo",
      "spell": "39:6-39:9|39:1-39:21|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 15303199746890529322,
      "detailed_name": "void vector<Z2, allocator<Z2>>::clear()",
      "qual_name_offset": 5,
      "short_name": "clear",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 6,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["27:8-27:13|27:3-27:15|1|-1"],
      "derived": [],
      "uses": []
    }, {
      "usr": 15522553613924683858,
      "detailed_name": "void foo(float Value)",
      "qual_name_offset": 5,
      "short_name": "foo",
      "bases": [],
      "vars": [12829778169986898163],
      "callees": [],
      "kind": 12,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["43:6-43:9|43:1-43:50|1|-1"],
      "derived": [],
      "uses": []
    }, {
      "usr": 17227714546476149620,
      "detailed_name": "void vector<T, Alloc>::clear()",
      "qual_name_offset": 5,
      "short_name": "clear",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 6,
      "parent_kind": 0,
      "storage": 0,
      "declarations": ["13:8-13:13|13:3-13:15|1|-1"],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
      "usr": 387107131966460226,
      "detailed_name": "struct Z1 {}",
      "qual_name_offset": 7,
      "short_name": "Z1",
      "spell": "19:8-19:10|19:1-19:13|2|-1",
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
      "uses": ["21:23-21:25|4|-1", "32:8-32:10|4|-1"]
    }, {
      "usr": 2287319947425285985,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "38:20-38:21|38:11-38:21|2|-1",
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
      "uses": ["39:10-39:11|4|-1"]
    }, {
      "usr": 2838478884129622301,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "11:19-11:20|11:10-11:20|2|-1",
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
      "uses": ["11:49-11:50|4|-1"]
    }, {
      "usr": 4288100429274082808,
      "detailed_name": "class vector {}",
      "qual_name_offset": 6,
      "short_name": "vector",
      "spell": "17:7-17:13|17:1-17:20|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [8409568383899457664],
      "uses": ["31:1-31:7|4|-1"]
    }, {
      "usr": 4391657975213906247,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "4:19-4:20|4:10-4:20|2|-1",
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
      "uses": ["5:16-5:17|4|-1"]
    }, {
      "usr": 4499014130156038222,
      "detailed_name": "class function",
      "qual_name_offset": 6,
      "short_name": "function",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 0,
      "declarations": ["2:7-2:15|2:1-2:15|1|-1"],
      "derived": [],
      "instances": [],
      "uses": []
    }, {
      "usr": 4983446252340304348,
      "detailed_name": "class vector<Z2, allocator<Z2>> {}",
      "qual_name_offset": 6,
      "short_name": "vector",
      "spell": "26:7-26:13|26:1-28:2|2|-1",
      "bases": [15638311286894266942],
      "funcs": [15303199746890529322],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [4257427079291996842],
      "uses": ["33:1-33:7|4|-1"]
    }, {
      "usr": 10018309536213112739,
      "detailed_name": "Args",
      "qual_name_offset": 0,
      "short_name": "Args",
      "spell": "4:34-4:38|4:10-5:30|2|-1",
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
      "uses": ["5:18-5:22|4|-1"]
    }, {
      "usr": 10739827112898505249,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "9:19-9:20|9:10-9:37|2|-1",
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
      "usr": 11633485747518002690,
      "detailed_name": "class allocator",
      "qual_name_offset": 6,
      "short_name": "allocator",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 0,
      "declarations": ["9:28-9:37|9:22-9:37|1|-1"],
      "derived": [],
      "instances": [],
      "uses": ["11:39-11:48|4|-1"]
    }, {
      "usr": 13641542451932043374,
      "detailed_name": "class function {}",
      "qual_name_offset": 6,
      "short_name": "function",
      "spell": "5:7-5:15|5:1-5:30|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [11939239848078880065],
      "uses": ["7:1-7:9|4|-1"]
    }, {
      "usr": 13826100924361265353,
      "detailed_name": "struct Z2 {}",
      "qual_name_offset": 7,
      "short_name": "Z2",
      "spell": "23:8-23:10|23:1-23:13|2|-1",
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
      "uses": ["26:14-26:16|4|-1", "33:8-33:10|4|-1"]
    }, {
      "usr": 14305091532438931060,
      "detailed_name": "enum Enum {}",
      "qual_name_offset": 5,
      "short_name": "Enum",
      "spell": "35:6-35:10|35:1-37:2|2|-1",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [{
          "L": 9389934145478267315,
          "R": -1
        }, {
          "L": 7252148324539897019,
          "R": -1
        }],
      "alias_of": 0,
      "kind": 10,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [7292940207203676942],
      "uses": ["38:30-38:34|4|-1"]
    }, {
      "usr": 14347450163289721749,
      "detailed_name": "Alloc",
      "qual_name_offset": 0,
      "short_name": "Alloc",
      "spell": "11:31-11:36|11:10-14:2|2|-1",
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
      "usr": 15638311286894266942,
      "detailed_name": "class vector {}",
      "qual_name_offset": 6,
      "short_name": "vector",
      "spell": "12:7-12:13|12:1-14:2|2|-1",
      "bases": [],
      "funcs": [17227714546476149620],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [4983446252340304348],
      "instances": [14712970351431237480, 2477688518054054614],
      "uses": ["21:16-21:22|4|-1", "30:1-30:7|4|-1", "32:1-32:7|4|-1"]
    }, {
      "usr": 16550298761252887443,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "16:19-16:20|16:10-17:20|2|-1",
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
      "uses": ["17:14-17:15|4|-1"]
    }, {
      "usr": 17562268084967245363,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "1:16-1:17|1:10-2:15|2|-1",
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
      "usr": 2477688518054054614,
      "detailed_name": "vector<Z1, allocator<Z1>> vz1",
      "qual_name_offset": 26,
      "short_name": "vz1",
      "spell": "32:12-32:15|32:1-32:15|2|-1",
      "type": 15638311286894266942,
      "kind": 13,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 4257427079291996842,
      "detailed_name": "vector<Z2, allocator<Z2>> vz2",
      "qual_name_offset": 26,
      "short_name": "vz2",
      "spell": "33:12-33:15|33:1-33:15|2|-1",
      "type": 4983446252340304348,
      "kind": 13,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 7252148324539897019,
      "detailed_name": "Enum1",
      "qual_name_offset": 0,
      "short_name": "Enum1",
      "spell": "36:10-36:15|36:10-36:15|2|-1",
      "type": 14305091532438931060,
      "kind": 22,
      "parent_kind": 10,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 7292940207203676942,
      "detailed_name": "Enum <unnamed>",
      "qual_name_offset": 5,
      "short_name": "<unnamed>",
      "spell": "38:34-38:35|38:30-41:27|2|-1",
      "type": 14305091532438931060,
      "kind": 253,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 7855006249566188515,
      "detailed_name": "int E",
      "qual_name_offset": 4,
      "short_name": "E",
      "spell": "38:40-38:41|38:36-41:27|2|-1",
      "type": 0,
      "kind": 253,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 8409568383899457664,
      "detailed_name": "vector<int *, allocator<int *>> vip",
      "qual_name_offset": 32,
      "short_name": "vip",
      "spell": "31:14-31:17|31:1-31:17|2|-1",
      "type": 4288100429274082808,
      "kind": 13,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 9389934145478267315,
      "detailed_name": "Enum0",
      "qual_name_offset": 0,
      "short_name": "Enum0",
      "spell": "36:3-36:8|36:3-36:8|2|-1",
      "type": 14305091532438931060,
      "kind": 22,
      "parent_kind": 10,
      "storage": 0,
      "declarations": [],
      "uses": ["43:20-43:25|4|-1"]
    }, {
      "usr": 10707811920683213484,
      "detailed_name": "int I",
      "qual_name_offset": 4,
      "short_name": "I",
      "spell": "38:27-38:28|38:23-38:28|2|-1",
      "type": 0,
      "kind": 253,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 11939239848078880065,
      "detailed_name": "function<void (int)> f",
      "qual_name_offset": 21,
      "short_name": "f",
      "spell": "7:21-7:22|7:1-7:22|2|-1",
      "type": 13641542451932043374,
      "kind": 13,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 12781399699941188794,
      "detailed_name": "static const int kOnst",
      "qual_name_offset": 17,
      "short_name": "kOnst",
      "hover": "static const int kOnst = 7",
      "spell": "41:18-41:23|41:1-41:27|2|-1",
      "type": 0,
      "kind": 13,
      "parent_kind": 1,
      "storage": 2,
      "declarations": [],
      "uses": ["43:27-43:32|12|-1"]
    }, {
      "usr": 12829778169986898163,
      "detailed_name": "float Value",
      "qual_name_offset": 6,
      "short_name": "Value",
      "spell": "43:44-43:49|43:38-43:49|2|-1",
      "type": 0,
      "kind": 253,
      "parent_kind": 12,
      "storage": 0,
      "declarations": [],
      "uses": []
    }, {
      "usr": 14712970351431237480,
      "detailed_name": "vector<char, allocator<char>> vc",
      "qual_name_offset": 30,
      "short_name": "vc",
      "spell": "30:14-30:16|30:1-30:16|2|-1",
      "type": 15638311286894266942,
      "kind": 13,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "uses": []
    }]
}
*/
