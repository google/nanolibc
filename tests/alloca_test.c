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

/* Make sure alloca() works */

#include <alloca.h>
#include <assert.h>
#include <stdio.h>

static unsigned compute_sum(unsigned x) {
  unsigned *t = (unsigned *)alloca((x + 1) * sizeof(unsigned));
  unsigned i;
  printf("  (Computing sum 0..%d ...)\n", x);
  t[0] = 0;
  for (i = 1; i <= x; ++i) t[i] = t[i - 1] + i;
  printf("  (Computed sum 0..%d)\n", x);
  return t[x];
}

int main() {
  unsigned i;
  for (i = 1; i <= 300; ++i) {
    unsigned sigma = compute_sum(i);
    assert(sigma == ((i * (i + 1)) >> 1));
    printf("Sum 0..%d = %u\n", i, sigma);
  }
  return 0;
}
