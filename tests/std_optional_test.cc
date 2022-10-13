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

/* Unit tests for std::optional */

#include <stdio.h>

#include <optional>
#include <string>

#include "third_party/nanolibc/tests/test_utils/nnlc_test.h"

namespace {

std::optional<std::string> MaybeString(bool return_string) {
  if (return_string) {
    return std::string("Hello, World!");
  }
  return {};
}

void TestOptional() {
  auto opt_str = MaybeString(true);
  ASSERT(opt_str.has_value());
  ASSERT(opt_str.value() == "Hello, World!");
  opt_str = MaybeString(false);
  ASSERT(!opt_str.has_value());
  // Verify default constructor has no value.
  ASSERT(!std::optional<int>().has_value());
}

bool HasValue(std::optional<uint32_t> maybe_u32) {
  return maybe_u32.has_value();
}

void TestNullopt() {
  ASSERT(HasValue(3));
  ASSERT(!HasValue(std::nullopt));
}

}  // namespace

int main() {
  TestOptional();
  TestNullopt();
  printf("PASSED\n");
  return 0;
}
