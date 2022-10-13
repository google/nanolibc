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

/* Unit tests for std::string */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <utility>
#include <vector>

#include "third_party/nanolibc/tests/test_utils/nnlc_test.h"

namespace {

using Uint8Vect = std::vector<uint8_t>;
using StrPair = std::pair<std::string, Uint8Vect>;

void TestPair() {
  Uint8Vect u1;
  u1.push_back(1);
  u1.push_back(2);
  StrPair p1("Hello", u1);
  ASSERT(p1.first == "Hello");
  ASSERT(p1.second.size() == 2);
  ASSERT(p1.second[0] == 1);
  ASSERT(p1.second[1] == 2);
  StrPair p2(p1.first, p1.second);
  ASSERT(p2.first == "Hello");
  ASSERT(p2.second.size() == 2);
  ASSERT(p2.second[0] == 1);
  ASSERT(p2.second[1] == 2);
  StrPair p3;
  ASSERT(p3.first.empty());
  ASSERT(p3.second.empty());
  // NOLINTBEGIN
  StrPair p4(p1.first, std::move(p1.second));
  ASSERT(p1.second.empty());
  // NOLINTEND
  ASSERT(p1.first == "Hello");
  ASSERT(p4.first == "Hello");
  ASSERT(p4.second.size() == 2);
  ASSERT(p4.second[0] == 1);
  ASSERT(p4.second[1] == 2);
  // NOLINTBEGIN
  StrPair p5(std::move(p2.first), p2.second);
  ASSERT(p2.first.empty());
  // NOLINTEND
  ASSERT(p2.second.size() == 2);
  ASSERT(p2.second[0] == 1);
  ASSERT(p2.second[1] == 2);
  ASSERT(p5.first == "Hello");
  ASSERT(p5.second.size() == 2);
  ASSERT(p5.second[0] == 1);
  ASSERT(p5.second[1] == 2);
  // NOLINTBEGIN
  StrPair p6(std::move(p5.first), std::move(p5.second));
  ASSERT(p5.first.empty());
  ASSERT(p5.second.empty());
  // NOLINTEND
  ASSERT(p6.second.size() == 2);
  ASSERT(p6.second[0] == 1);
  ASSERT(p6.second[1] == 2);
  StrPair p7(p6);
  ASSERT(p7.first == "Hello");
  ASSERT(p7.second.size() == 2);
  ASSERT(p7.second[0] == 1);
  ASSERT(p7.second[1] == 2);
  // NOLINTBEGIN
  StrPair p8(std::move(p6.first), std::move(p6.second));
  ASSERT(p6.first.empty());
  ASSERT(p6.second.empty());
  // NOLINTEND
  ASSERT(p8.first == "Hello");
  ASSERT(p8.second.size() == 2);
  ASSERT(p8.second[0] == 1);
  ASSERT(p8.second[1] == 2);
  StrPair p9 = p8;
  ASSERT(p9.first == "Hello");
  ASSERT(p9.second.size() == 2);
  ASSERT(p9.second[0] == 1);
  ASSERT(p9.second[1] == 2);
  // NOLINTBEGIN
  StrPair p10 = std::move(p7);
  ASSERT(p7.first.empty());
  ASSERT(p7.second.empty());
  // NOLINTEND
  ASSERT(p10.first == "Hello");
  ASSERT(p10.second.size() == 2);
  ASSERT(p10.second[0] == 1);
  ASSERT(p10.second[1] == 2);
}

}  // namespace

int main() {
  TestPair();
  printf("PASSED\n");
  return 0;
}
