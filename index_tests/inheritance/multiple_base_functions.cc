struct Base0 {
  virtual ~Base0() { }
};
struct Base1 {
  virtual ~Base1() { }
};
struct Derived : Base0, Base1 {
  ~Derived() override { }
};

/*
OUTPUT:
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 4535810436732871710,
      "detailed_name": "virtual Base0::~Base0() noexcept",
      "qual_name_offset": 8,
      "short_name": "~Base0",
      "spell": "2:11-2:17|2:3-2:23|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 6,
      "parent_kind": 23,
      "storage": 0,
      "declarations": [],
      "derived": [12766527836341732282],
      "uses": ["8:3-8:11|292|-1"]
    }, {
      "usr": 12382325882880223873,
      "detailed_name": "virtual Base1::~Base1() noexcept",
      "qual_name_offset": 8,
      "short_name": "~Base1",
      "spell": "5:11-5:17|5:3-5:23|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 6,
      "parent_kind": 23,
      "storage": 0,
      "declarations": [],
      "derived": [12766527836341732282],
      "uses": ["8:3-8:11|292|-1"]
    }, {
      "usr": 12766527836341732282,
      "detailed_name": "Derived::~Derived() noexcept override",
      "qual_name_offset": 0,
      "short_name": "~Derived",
      "spell": "8:3-8:11|8:3-8:26|2|-1",
      "bases": [4535810436732871710, 12382325882880223873],
      "vars": [],
      "callees": ["8:3-8:11|4535810436732871710|3|292", "8:3-8:11|12382325882880223873|3|292"],
      "kind": 6,
      "parent_kind": 23,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
      "usr": 9739895618756211179,
      "detailed_name": "struct Derived : Base0, Base1 {}",
      "qual_name_offset": 7,
      "short_name": "Derived",
      "spell": "7:8-7:15|7:1-9:2|2|-1",
      "bases": [14976383007658795245, 15629658110148285751],
      "funcs": [12766527836341732282],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["8:4-8:11|4|-1"]
    }, {
      "usr": 14976383007658795245,
      "detailed_name": "struct Base0 {}",
      "qual_name_offset": 7,
      "short_name": "Base0",
      "spell": "1:8-1:13|1:1-3:2|2|-1",
      "bases": [],
      "funcs": [4535810436732871710],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [9739895618756211179],
      "instances": [],
      "uses": ["2:12-2:17|4|-1", "7:18-7:23|4|-1"]
    }, {
      "usr": 15629658110148285751,
      "detailed_name": "struct Base1 {}",
      "qual_name_offset": 7,
      "short_name": "Base1",
      "spell": "4:8-4:13|4:1-6:2|2|-1",
      "bases": [],
      "funcs": [12382325882880223873],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 1,
      "declarations": [],
      "derived": [9739895618756211179],
      "instances": [],
      "uses": ["5:12-5:17|4|-1", "7:25-7:30|4|-1"]
    }],
  "usr2var": []
}
*/