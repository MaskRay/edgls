#include "make_functions.h"

template <typename T, typename... Args>
T* MakeUnique(Args&&... args) {
  return nullptr;
}

template <typename T, typename... Args>
T* maKE_NoRefs(Args... args) {
  return nullptr;
}

void caller22() {
  MakeUnique<Foobar>();
  MakeUnique<Foobar>(1);
  MakeUnique<Foobar>(1, new Bar(), nullptr);
  maKE_NoRefs<Foobar>(1, new Bar(), nullptr);
}

// TODO: Eliminate the extra entries in the "types" array here. They come from
// the template function definitions.

// Foobar is defined in a separate file to ensure that we can attribute
// MakeUnique calls across translation units.

/*
OUTPUT: make_functions.h
{
  "includes": [],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 5196039125282248264,
      "detailed_name": "Foobar::Foobar()",
      "qual_name_offset": 0,
      "short_name": "Foobar",
      "spell": "5:3-5:9|5:3-5:14|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 9,
      "parent_kind": 5,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 8432545353196237887,
      "detailed_name": "Foobar::Foobar(int &&, Bar *, bool *)",
      "qual_name_offset": 0,
      "short_name": "Foobar",
      "spell": "7:3-7:9|7:3-7:32|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 9,
      "parent_kind": 5,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 10699524074024683167,
      "detailed_name": "Foobar::Foobar(int)",
      "qual_name_offset": 0,
      "short_name": "Foobar",
      "spell": "6:3-6:9|6:3-6:17|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 9,
      "parent_kind": 5,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 14217857536668225362,
      "detailed_name": "Foobar::Foobar(int, Bar *, bool *)",
      "qual_name_offset": 0,
      "short_name": "Foobar",
      "spell": "8:3-8:9|8:3-8:30|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 9,
      "parent_kind": 5,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }],
  "usr2type": [{
      "usr": 1226310018180472538,
      "detailed_name": "class Foobar {}",
      "qual_name_offset": 6,
      "short_name": "Foobar",
      "spell": "3:7-3:13|3:1-9:2|2|-1",
      "bases": [],
      "funcs": [5196039125282248264, 10699524074024683167, 8432545353196237887, 14217857536668225362],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 1,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["5:3-5:9|4|-1", "6:3-6:9|4|-1", "7:3-7:9|4|-1", "8:3-8:9|4|-1"]
    }, {
      "usr": 5615179467931063350,
      "detailed_name": "struct Bar {}",
      "qual_name_offset": 7,
      "short_name": "Bar",
      "spell": "1:8-1:11|1:1-1:14|2|-1",
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
      "uses": ["7:17-7:20|4|-1", "8:15-8:18|4|-1"]
    }],
  "usr2var": []
}
OUTPUT: make_functions.cc
{
  "includes": [{
      "line": 0,
      "resolved_path": "&make_functions.h"
    }],
  "skipped_ranges": [],
  "usr2func": [{
      "usr": 2869659483791440037,
      "detailed_name": "void caller22()",
      "qual_name_offset": 5,
      "short_name": "caller22",
      "spell": "13:6-13:14|13:1-18:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": ["14:3-14:13|11251043317432900264|3|36", "15:3-15:13|11251043317432900264|3|36", "16:25-16:28|13428682614327868979|3|36", "16:25-16:28|10185684319667591300|3|36", "16:3-16:13|11251043317432900264|3|36", "17:26-17:29|13428682614327868979|3|36", "17:26-17:29|10185684319667591300|3|36", "17:3-17:14|10545421682449281838|3|36"],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": []
    }, {
      "usr": 10185684319667591300,
      "detailed_name": "void *operator new(unsigned long)",
      "qual_name_offset": 6,
      "short_name": "operator new",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 0,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["16:25-16:28|36|-1", "17:26-17:29|36|-1"]
    }, {
      "usr": 10545421682449281838,
      "detailed_name": "T *maKE_NoRefs(Args ...args)",
      "qual_name_offset": 3,
      "short_name": "maKE_NoRefs",
      "spell": "9:4-9:15|9:1-11:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["17:3-17:14|36|-1"]
    }, {
      "usr": 11251043317432900264,
      "detailed_name": "T *MakeUnique(Args &&...args)",
      "qual_name_offset": 3,
      "short_name": "MakeUnique",
      "spell": "4:4-4:14|4:1-6:2|2|-1",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 1,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["14:3-14:13|36|-1", "15:3-15:13|36|-1", "16:3-16:13|36|-1"]
    }, {
      "usr": 13428682614327868979,
      "detailed_name": "void operator delete(void *, unsigned long) noexcept",
      "qual_name_offset": 5,
      "short_name": "operator delete",
      "bases": [],
      "vars": [],
      "callees": [],
      "kind": 12,
      "parent_kind": 0,
      "storage": 0,
      "declarations": [],
      "derived": [],
      "uses": ["16:25-16:28|36|-1", "17:26-17:29|36|-1"]
    }],
  "usr2type": [{
      "usr": 374733647709627751,
      "detailed_name": "Args",
      "qual_name_offset": 0,
      "short_name": "Args",
      "spell": "8:35-8:39|8:11-11:2|2|-1",
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
      "uses": ["9:16-9:20|4|-1"]
    }, {
      "usr": 1226310018180472538,
      "detailed_name": "class Foobar {}",
      "qual_name_offset": 6,
      "short_name": "Foobar",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 5,
      "parent_kind": 0,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["14:14-14:20|4|-1", "15:14-15:20|4|-1", "16:14-16:20|4|-1", "17:15-17:21|4|-1"]
    }, {
      "usr": 5615179467931063350,
      "detailed_name": "struct Bar {}",
      "qual_name_offset": 7,
      "short_name": "Bar",
      "bases": [],
      "funcs": [],
      "types": [],
      "vars": [],
      "alias_of": 0,
      "kind": 23,
      "parent_kind": 0,
      "declarations": [],
      "derived": [],
      "instances": [],
      "uses": ["16:29-16:32|4|-1", "17:30-17:33|4|-1"]
    }, {
      "usr": 10297492663436760053,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "3:20-3:21|3:11-3:21|2|-1",
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
      "uses": ["4:1-4:2|4|-1"]
    }, {
      "usr": 11806818257063572040,
      "detailed_name": "Args",
      "qual_name_offset": 0,
      "short_name": "Args",
      "spell": "3:35-3:39|3:11-6:2|2|-1",
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
      "uses": ["4:15-4:19|4|-1"]
    }, {
      "usr": 17796470433143773246,
      "detailed_name": "T",
      "qual_name_offset": 0,
      "short_name": "T",
      "spell": "8:20-8:21|8:11-8:21|2|-1",
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
      "uses": ["9:1-9:2|4|-1"]
    }],
  "usr2var": []
}
*/
