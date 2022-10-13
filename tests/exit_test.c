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

/* test exit() */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "third_party/nanolibc/tests/test_utils/nnlc_test.h"

static void test_exit() {
  printf("Now exitting with exit code 42...\n");
  exit(42);
}

int main() {
  test_exit();

  ASSERT(NULL == "exit failed!");
  return -1;
}
