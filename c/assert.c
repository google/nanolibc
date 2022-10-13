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

/*
 * Implementation of assert.h functions.
 */

#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

/* assertion function called when #include <assert.h> was used */
void _nnlc_assert(int val, const char *expr, const char *fname, int line,
                  const char *funcname) {
  if (val) return;

  fprintf(stderr, "Assertion '%s' failed in %s (%s:%d)\n", expr, funcname,
          fname, line);
  abort();
}

/* for who doesn't #include <assert.h>, provide user with a default
 * assert() function to link with */
void _nanolibc_default_assert(int val) {
  if (val) return;

  fputs("Assertion failed!\n", stderr);
  abort();
}

#undef assert
void assert() __attribute__((weak, alias("_nanolibc_default_assert")));
