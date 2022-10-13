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

/* Unit tests for std::hash */

#include <stdio.h>

#include <functional>
#include <string>

#include "third_party/nanolibc/tests/test_utils/nnlc_test.h"

namespace {

size_t CountBits(size_t value) {
  size_t bits = 0;
  while (value != 0) {
    if (value & 1) {
      bits++;
    }
    value >>= 1;
  }
  return bits;
}

void TestHash() {
  std::string s = "Test";
  std::hash<std::string> h;
  size_t hash = h(s);
  ASSERT(hash != 0);
  size_t bits = 0;
  // Use Law of Large Numbers to keep this test from being flaky.
  constexpr size_t kIterations = 1000;
  for (size_t i = 0; i < kIterations; i++) {
    bits += CountBits(h(s + std::to_string(i)));
  }
  // Check that bits is > 1 in 4.
  ASSERT(bits > kIterations * 8 * sizeof(size_t) >> 2);
  // Check that bits is < 3 in 4.
  ASSERT(bits <
         kIterations * 8 * ((sizeof(size_t) >> 2) + (sizeof(size_t) >> 1)));
}

}  // namespace

int main() {
  TestHash();
  printf("PASSED\n");
  return 0;
}
