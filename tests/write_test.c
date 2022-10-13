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

// Test the write function.
//
#include <unistd.h>

#include "third_party/nanolibc/tests/test_utils/nnlc_test.h"

int main() {
  char message1[] = "Hello, ";
  char message2[] = "World!\n";

  write(STDOUT_FILENO, message1, sizeof(message1) - 1);
  write(STDERR_FILENO, message2, sizeof(message2) - 1);
  return 0;
}

