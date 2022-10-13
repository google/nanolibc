//  Copyright 2022 Google LLC.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/* Unit tests for std::unordered_map */

#include <stdio.h>
#include <stdlib.h>

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "third_party/nanolibc/tests/test_utils/nnlc_test.h"

namespace {

template <typename T>
class Widget {
 public:
  Widget() : name_(std::string()), value_(std::move(T())) {}
  Widget(const std::string& name, const T& value)
      : name_(name), value_(value) {}
  const std::string& Name() { return name_; }
  const std::string& Value() { return value_; }

 private:
  std::string name_ = "";
  T value_ = std::move(T());
};

// For now, we only use strings as keys.
using StringWidget = Widget<std::string>;
using StringMap = std::unordered_map<std::string, StringWidget>;

// Test constructors and methods of the Entry inner class.
void TestUnorderedMapOfStringWidgets() {
  StringMap map;
  StringWidget alice("Alice", "CEO");
  StringWidget bob("Bob", "Admin");
  StringWidget& widget = map[alice.Name()];
  widget = alice;
  map[bob.Name()] = bob;
  ASSERT(alice.Value() == map[alice.Name()].Value());
  ASSERT(bob.Value() == map[bob.Name()].Value());
  ASSERT(map[alice.Name()].Name() == "Alice");
  ASSERT(map[alice.Name()].Value() == "CEO");
  ASSERT(map[bob.Name()].Name() == "Bob");
  ASSERT(map[bob.Name()].Value() == "Admin");
}

void TestFindAndErase() {
  // Map of hashes to the strings that hashes to that value.
  std::unordered_map<size_t, std::unique_ptr<std::string>> map;  // NOLINT
  std::hash<std::string> hasher;
  std::string s1 = "Hello, ";
  std::string s2 = "World!";
  size_t hash1 = hasher(s1);
  size_t hash2 = hasher(s2);
  map[hash1] = std::make_unique<std::string>(s1);
  map[hash2] = std::make_unique<std::string>(s2);
  ASSERT(*map[hash1] == s1);
  ASSERT(*map[hash2] == s2);
  ASSERT(map[hash1].get() != &s1);
  ASSERT(map[hash2].get() != &s2);
  auto it1 = map.find(hash1);
  auto it2 = map.find(hash2);
  ASSERT(it1 != map.end());
  ASSERT(it2 != map.end());
  ASSERT(*it1->second == s1);
  ASSERT(*it2->second == s2);
  map.erase(hash1);
  it1 = map.find(hash1);
  ASSERT(it1 == map.end());
  it2 = map.find(hash2);
  ASSERT(it2 != map.end());
  ASSERT(*map[hash2] == s2);
}

void TestRandomIndertAndErase() {
  constexpr size_t kNumEntries = 1000;
  std::unordered_map<size_t, size_t> map;  // NOLINT
  for (size_t i = 0; i < kNumEntries; ++i) {
    map[i] = i + 1;
  }
  bool deleted[kNumEntries] = {
      false,
  };
  std::hash<size_t> hasher;
  for (size_t i = 0; i < 10 * kNumEntries; ++i) {
    size_t j = hasher(i) % kNumEntries;
    if (deleted[j]) {
      // Recreate it.
      ASSERT(map.find(j) == map.end());
      map[j] = j + 1;
      deleted[j] = false;
    } else {
      // Delete it.
      ASSERT(map.find(j) != map.end());
      ASSERT(map[j] == j + 1);
      map.erase(j);
      deleted[j] = true;
    }
  }
}

}  // namespace

int main() {
  TestUnorderedMapOfStringWidgets();
  TestFindAndErase();
  TestRandomIndertAndErase();
  printf("PASSED\n");
  return 0;
}
