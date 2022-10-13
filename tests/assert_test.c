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

/* Test assert() */

#include <stdio.h>
#include <assert.h>

static void test_macro_assert() {
  assert(1 != 0);
  assert(1 == 1);

  assert(2 == 3); /* expected to fail */

  fprintf(stderr, "You should never see this message!\n");
}

int main() {
  test_macro_assert();
  return 0;
}
