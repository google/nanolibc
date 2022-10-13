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

/* Test strtoX */

#include <stdio.h>
#include <stdlib.h>

#include "third_party/nanolibc/tests/test_utils/nnlc_test.h"

/*
 * Tests for strtol/strtoul
 */

#define OFFS_SENTINEL -1
static void test_strtol(const char *s, int base, int oexpect,
                        long int vexpect) {
  static const char *sentinel = "<SENTINEL>";
  long int tst;
  const char *pexpect = (oexpect != OFFS_SENTINEL) ? (s + oexpect) : sentinel;
  const char *ptst = sentinel;
  int err = 0;

  /* We know that if base is invalid, we should expect the sentinel
   * for endptr, override params */
  if ((base != 0) && ((base < 2) || (base > 36))) {
    pexpect = sentinel;
    vexpect = 0;
  }

  tst = strtol(s, (char **)&ptst, base);

  if (vexpect != tst) {
    fprintf(stderr, "strtol('%s', _, %d): retval exp %ld, got %ld\n", s, base,
            vexpect, tst);
    err = 1;
  }

  if (pexpect != ptst) {
    fprintf(stderr, "strtol('%s', &endptr, %d): endptr exp @%p, got @%p\n", s,
            base, pexpect, ptst);
    fprintf(stderr, "  (endptr exp '%s', got '%s')\n", pexpect, ptst);
    err = 1;
  }

  ASSERT(!err);
}

static void strtol_tests() {
  /* Test invalid convs */
  {
    static const int bases[] = {0, 1, 2, 5, 7, 8, 9, 10, 11, 16, 36, 37};
    unsigned i;
    for (i = 0; i < sizeof(bases) / sizeof(int); ++i) {
      int b = bases[i];

      test_strtol("", b, 0, 0);
      test_strtol(" ", b, 0, 0);
      test_strtol("  ", b, 0, 0);

      test_strtol("*1234{", b, 0, 0);
      test_strtol("*01234{", b, 0, 0);
      test_strtol("*1234{", b, 0, 0);
      test_strtol("`01234{", b, 0, 0);
      test_strtol(" {1234", b, 0, 0);
      test_strtol("|1234[", b, 0, 0);
      test_strtol("/01234[", b, 0, 0);
      test_strtol("\"1234[", b, 0, 0);
      test_strtol("(01234[", b, 0, 0);
      test_strtol("[1234", b, 0, 0);
      test_strtol(" *1234{", b, 0, 0);
      test_strtol(" *01234{", b, 0, 0);
      test_strtol(" *1234{", b, 0, 0);
      test_strtol(" `01234{", b, 0, 0);
      test_strtol("  {1234", b, 0, 0);
      test_strtol(" |1234[", b, 0, 0);
      test_strtol(" /01234[", b, 0, 0);
      test_strtol(" \"1234[", b, 0, 0);
      test_strtol(" (01234[", b, 0, 0);
      test_strtol(" [1234", b, 0, 0);
      test_strtol("*1234{ ", b, 0, 0);
      test_strtol("*01234{ ", b, 0, 0);
      test_strtol("*1234{ ", b, 0, 0);
      test_strtol("`01234{ ", b, 0, 0);
      test_strtol(" {1234 ", b, 0, 0);
      test_strtol("|1234[ ", b, 0, 0);
      test_strtol("/01234[ ", b, 0, 0);
      test_strtol("\"1234[ ", b, 0, 0);
      test_strtol("(01234[ ", b, 0, 0);
      test_strtol("[1234 ", b, 0, 0);
      test_strtol(" *1234{ ", b, 0, 0);
      test_strtol(" *01234{ ", b, 0, 0);
      test_strtol(" *1234{ ", b, 0, 0);
      test_strtol(" `01234{ ", b, 0, 0);
      test_strtol("  {1234 ", b, 0, 0);
      test_strtol(" |1234[ ", b, 0, 0);
      test_strtol(" /01234[ ", b, 0, 0);
      test_strtol(" \"1234[ ", b, 0, 0);
      test_strtol(" (01234[ ", b, 0, 0);
      test_strtol(" [1234 ", b, 0, 0);

      test_strtol("", b, 0, 0);
      test_strtol(" ", b, 0, 0);
      test_strtol("  ", b, 0, 0);
      test_strtol("-", b, 0, 0);
      test_strtol(" -", b, 0, 0);
      test_strtol("  -", b, 0, 0);
      test_strtol("- ", b, 0, 0);
      test_strtol(" - ", b, 0, 0);
      test_strtol("  - ", b, 0, 0);
      test_strtol("-  ", b, 0, 0);
      test_strtol(" -  ", b, 0, 0);
      test_strtol("  -  ", b, 0, 0);
      test_strtol("+", b, 0, 0);
      test_strtol(" +", b, 0, 0);
      test_strtol("  +", b, 0, 0);
      test_strtol("+ ", b, 0, 0);
      test_strtol(" + ", b, 0, 0);
      test_strtol("  + ", b, 0, 0);
      test_strtol("+  ", b, 0, 0);
      test_strtol(" +  ", b, 0, 0);
      test_strtol("  +  ", b, 0, 0);

      test_strtol("+|1234{", b, 0, 0);
      test_strtol(" + )01234{", b, 0, 0);
      test_strtol("+ +1234{", b, 0, 0);
      test_strtol(" + -01234{", b, 0, 0);
      test_strtol(" - -1234", b, 0, 0);
      test_strtol(" + +1234[", b, 0, 0);
      test_strtol(" + -01234[", b, 0, 0);
      test_strtol(" - *1234[", b, 0, 0);
      test_strtol("-/01234[", b, 0, 0);
      test_strtol("$[x1234", b, 0, 0);

      test_strtol("- 0", b, 0, 0);
      test_strtol("- 0 ", b, 0, 0);
      test_strtol("+ 0", b, 0, 0);
      test_strtol("+ 0 ", b, 0, 0);

      /* check 0 in all bases */
      test_strtol("0", b, 1, 0);
      test_strtol(" 0", b, 2, 0);
      test_strtol("0 ", b, 1, 0);
      test_strtol(" 0 ", b, 2, 0);

      test_strtol("00", b, 2, 0);
      test_strtol(" 00", b, 3, 0);
      test_strtol("00 ", b, 2, 0);
      test_strtol(" 00 ", b, 3, 0);

      test_strtol("-0", b, 2, 0);
      test_strtol(" -0", b, 3, 0);
      test_strtol("-0 ", b, 2, 0);
      test_strtol(" -0 ", b, 3, 0);

      test_strtol("+0", b, 2, 0);
      test_strtol(" +0", b, 3, 0);
      test_strtol("+0 ", b, 2, 0);
      test_strtol(" +0 ", b, 3, 0);
    }
  }

  /* 01234 */
  test_strtol("01234", 0, 5, 01234);
  test_strtol("01234", 8, 5, 01234);
  test_strtol("01234", 10, 5, 1234);
  test_strtol("01234", 3, 3, 5);
  test_strtol("01234", 36, 5, 49360);

  test_strtol(" 01234", 0, 6, 01234);
  test_strtol(" 01234", 8, 6, 01234);
  test_strtol(" 01234", 10, 6, 1234);
  test_strtol(" 01234", 3, 4, 5);
  test_strtol(" 01234", 36, 6, 49360);

  test_strtol("01234 ", 0, 5, 01234);
  test_strtol("01234 ", 8, 5, 01234);
  test_strtol("01234 ", 10, 5, 1234);
  test_strtol("01234 ", 3, 3, 5);
  test_strtol("01234 ", 36, 5, 49360);

  test_strtol(" 01234 ", 0, 6, 01234);
  test_strtol(" 01234 ", 8, 6, 01234);
  test_strtol(" 01234 ", 10, 6, 1234);
  test_strtol(" 01234 ", 3, 4, 5);
  test_strtol(" 01234 ", 36, 6, 49360);

  test_strtol("-01234", 0, 6, -01234);
  test_strtol("-01234", 8, 6, -01234);
  test_strtol("-01234", 10, 6, -1234);
  test_strtol("-01234", 3, 4, -5);
  test_strtol("-01234", 36, 6, -49360);

  test_strtol(" -01234", 0, 7, -01234);
  test_strtol(" -01234", 8, 7, -01234);
  test_strtol(" -01234", 10, 7, -1234);
  test_strtol(" -01234", 3, 5, -5);
  test_strtol(" -01234", 36, 7, -49360);

  test_strtol("- 01234", 0, 0, 0);
  test_strtol("- 01234", 8, 0, 0);
  test_strtol("- 01234", 10, 0, 0);
  test_strtol("- 01234", 3, 0, 0);
  test_strtol("- 01234", 36, 0, 0);

  test_strtol("-01234 ", 0, 6, -01234);
  test_strtol("-01234 ", 8, 6, -01234);
  test_strtol("-01234 ", 10, 6, -1234);
  test_strtol("-01234 ", 3, 4, -5);
  test_strtol("-01234 ", 36, 6, -49360);

  test_strtol("- 01234 ", 0, 0, 0);
  test_strtol("- 01234 ", 8, 0, 0);
  test_strtol("- 01234 ", 10, 0, 0);
  test_strtol("- 01234 ", 3, 0, 0);
  test_strtol("- 01234 ", 36, 0, 0);

  test_strtol(" -01234 ", 0, 7, -01234);
  test_strtol(" -01234 ", 8, 7, -01234);
  test_strtol(" -01234 ", 10, 7, -1234);
  test_strtol(" -01234 ", 3, 5, -5);
  test_strtol(" -01234 ", 36, 7, -49360);

  test_strtol("+01234", 0, 6, 01234);
  test_strtol("+01234", 8, 6, 01234);
  test_strtol("+01234", 10, 6, 1234);
  test_strtol("+01234", 3, 4, 5);
  test_strtol("+01234", 36, 6, 49360);

  test_strtol(" +01234", 0, 7, 01234);
  test_strtol(" +01234", 8, 7, 01234);
  test_strtol(" +01234", 10, 7, 1234);
  test_strtol(" +01234", 3, 5, 5);
  test_strtol(" +01234", 36, 7, 49360);

  test_strtol("+ 01234", 0, 0, 0);
  test_strtol("+ 01234", 8, 0, 0);
  test_strtol("+ 01234", 10, 0, 0);
  test_strtol("+ 01234", 3, 0, 0);
  test_strtol("+ 01234", 36, 0, 0);

  test_strtol("+01234 ", 0, 6, 01234);
  test_strtol("+01234 ", 8, 6, 01234);
  test_strtol("+01234 ", 10, 6, 1234);
  test_strtol("+01234 ", 3, 4, 5);
  test_strtol("+01234 ", 36, 6, 49360);

  test_strtol("+ 01234 ", 0, 0, 0);
  test_strtol("+ 01234 ", 8, 0, 0);
  test_strtol("+ 01234 ", 10, 0, 0);
  test_strtol("+ 01234 ", 3, 0, 0);
  test_strtol("+ 01234 ", 36, 0, 0);

  test_strtol(" +01234 ", 0, 7, 01234);
  test_strtol(" +01234 ", 8, 7, 01234);
  test_strtol(" +01234 ", 10, 7, 1234);
  test_strtol(" +01234 ", 3, 5, 5);
  test_strtol(" +01234 ", 36, 7, 49360);

  /* 1234 */
  test_strtol("1234", 0, 4, 1234);
  test_strtol("1234", 8, 4, 01234);
  test_strtol("1234", 10, 4, 1234);
  test_strtol("1234", 3, 2, 5);
  test_strtol("1234", 36, 4, 49360);

  test_strtol(" 1234", 0, 5, 1234);
  test_strtol(" 1234", 8, 5, 01234);
  test_strtol(" 1234", 10, 5, 1234);
  test_strtol(" 1234", 3, 3, 5);
  test_strtol(" 1234", 36, 5, 49360);

  test_strtol("1234 ", 0, 4, 1234);
  test_strtol("1234 ", 8, 4, 01234);
  test_strtol("1234 ", 10, 4, 1234);
  test_strtol("1234 ", 3, 2, 5);
  test_strtol("1234 ", 36, 4, 49360);

  test_strtol(" 1234 ", 0, 5, 1234);
  test_strtol(" 1234 ", 8, 5, 01234);
  test_strtol(" 1234 ", 10, 5, 1234);
  test_strtol(" 1234 ", 3, 3, 5);
  test_strtol(" 1234 ", 36, 5, 49360);

  test_strtol("-1234", 0, 5, -1234);
  test_strtol("-1234", 8, 5, -01234);
  test_strtol("-1234", 10, 5, -1234);
  test_strtol("-1234", 3, 3, -5);
  test_strtol("-1234", 36, 5, -49360);

  test_strtol(" -1234", 0, 6, -1234);
  test_strtol(" -1234", 8, 6, -01234);
  test_strtol(" -1234", 10, 6, -1234);
  test_strtol(" -1234", 3, 4, -5);
  test_strtol(" -1234", 36, 6, -49360);

  test_strtol("- 1234", 0, 0, 0);
  test_strtol("- 1234", 8, 0, 0);
  test_strtol("- 1234", 10, 0, 0);
  test_strtol("- 1234", 3, 0, 0);
  test_strtol("- 1234", 36, 0, 0);

  test_strtol("-1234 ", 0, 5, -1234);
  test_strtol("-1234 ", 8, 5, -01234);
  test_strtol("-1234 ", 10, 5, -1234);
  test_strtol("-1234 ", 3, 3, -5);
  test_strtol("-1234 ", 36, 5, -49360);

  test_strtol("- 1234 ", 0, 0, 0);
  test_strtol("- 1234 ", 8, 0, 0);
  test_strtol("- 1234 ", 10, 0, 0);
  test_strtol("- 1234 ", 3, 0, 0);
  test_strtol("- 1234 ", 36, 0, 0);

  test_strtol(" -1234 ", 0, 6, -1234);
  test_strtol(" -1234 ", 8, 6, -01234);
  test_strtol(" -1234 ", 10, 6, -1234);
  test_strtol(" -1234 ", 3, 4, -5);
  test_strtol(" -1234 ", 36, 6, -49360);

  test_strtol("+1234", 0, 5, 1234);
  test_strtol("+1234", 8, 5, 01234);
  test_strtol("+1234", 10, 5, 1234);
  test_strtol("+1234", 3, 3, 5);
  test_strtol("+1234", 36, 5, 49360);

  test_strtol(" +1234", 0, 6, 1234);
  test_strtol(" +1234", 8, 6, 01234);
  test_strtol(" +1234", 10, 6, 1234);
  test_strtol(" +1234", 3, 4, 5);
  test_strtol(" +1234", 36, 6, 49360);

  test_strtol("+ 1234", 0, 0, 0);
  test_strtol("+ 1234", 8, 0, 0);
  test_strtol("+ 1234", 10, 0, 0);
  test_strtol("+ 1234", 3, 0, 0);
  test_strtol("+ 1234", 36, 0, 0);

  test_strtol("+1234 ", 0, 5, 1234);
  test_strtol("+1234 ", 8, 5, 01234);
  test_strtol("+1234 ", 10, 5, 1234);
  test_strtol("+1234 ", 3, 3, 5);
  test_strtol("+1234 ", 36, 5, 49360);

  test_strtol("+ 1234 ", 0, 0, 0);
  test_strtol("+ 1234 ", 8, 0, 0);
  test_strtol("+ 1234 ", 10, 0, 0);
  test_strtol("+ 1234 ", 3, 0, 0);
  test_strtol("+ 1234 ", 36, 0, 0);

  test_strtol(" +1234 ", 0, 6, 1234);
  test_strtol(" +1234 ", 8, 6, 01234);
  test_strtol(" +1234 ", 10, 6, 1234);
  test_strtol(" +1234 ", 3, 4, 5);
  test_strtol(" +1234 ", 36, 6, 49360);

  /* 0x ... */
  test_strtol("0xabcd", 0, 6, 0xabcd);
  test_strtol("0Xabcd", 0, 6, 0xabcd);
  test_strtol("0xABCD", 0, 6, 0xabcd);
  test_strtol("0XABCD", 0, 6, 0xabcd);
  test_strtol("0xabcd", 16, 6, 0xabcd);
  test_strtol("0Xabcd", 16, 6, 0xabcd);
  test_strtol("0xABCD", 16, 6, 0xabcd);
  test_strtol("0XABCD", 16, 6, 0xabcd);
  // test_strtol("0xabcd", 33, 0, 0);  // BUG in eglibc?... allowed!
  // test_strtol("0Xabcd", 33, 0, 0);  // BUG in eglibc?... allowed!
  // test_strtol("0xABCD", 33, 0, 0);  // BUG in eglibc?... allowed!
  // test_strtol("0XABCD", 33, 0, 0);  // BUG in eglibc?... allowed!
  test_strtol("0xabcd", 34, 6, 44505265);
  test_strtol("0Xabcd", 34, 6, 44505265);
  test_strtol("0xABCD", 34, 6, 44505265);
  test_strtol("0XABCD", 34, 6, 44505265);

  test_strtol("0x01234", 0, 7, 0x1234);
  test_strtol("0x01234", 16, 7, 0x1234);
  test_strtol("0x01234", 8, 1, 0);
  test_strtol("0x01234", 10, 1, 0);
  test_strtol("0x01234", 3, 1, 0);
  test_strtol("0x01234", 36, 7, 1995433168);

  test_strtol(" 0x01234", 0, 8, 0x1234);
  test_strtol(" 0x01234", 16, 8, 0x1234);
  test_strtol(" 0x01234", 8, 2, 0);
  test_strtol(" 0x01234", 10, 2, 0);
  test_strtol(" 0x01234", 3, 2, 0);
  test_strtol(" 0x01234", 36, 8, 1995433168);

  test_strtol(" 0x01234 ", 0, 8, 0x1234);
  test_strtol(" 0x01234 ", 16, 8, 0x1234);
  test_strtol(" 0x01234 ", 8, 2, 0);
  test_strtol(" 0x01234 ", 10, 2, 0);
  test_strtol(" 0x01234 ", 3, 2, 0);
  test_strtol(" 0x01234 ", 36, 8, 1995433168);

  test_strtol("0x01234 ", 0, 7, 0x1234);
  test_strtol("0x01234 ", 16, 7, 0x1234);
  test_strtol("0x01234 ", 8, 1, 0);
  test_strtol("0x01234 ", 10, 1, 0);
  test_strtol("0x01234 ", 3, 1, 0);
  test_strtol("0x01234 ", 36, 7, 1995433168);

  test_strtol("-0x01234", 0, 8, -0x1234);
  test_strtol("-0x01234", 16, 8, -0x1234);
  test_strtol("-0x01234", 8, 2, 0);
  test_strtol("-0x01234", 10, 2, 0);
  test_strtol("-0x01234", 3, 2, 0);
  test_strtol("-0x01234", 36, 8, -1995433168);

  test_strtol("- 0x01234", 0, 0, 0);
  test_strtol("- 0x01234", 16, 0, 0);
  test_strtol("- 0x01234", 8, 0, 0);
  test_strtol("- 0x01234", 10, 0, 0);
  test_strtol("- 0x01234", 3, 0, 0);
  test_strtol("- 0x01234", 36, 0, 0);

  test_strtol("- 0x01234 ", 0, 0, 0);
  test_strtol("- 0x01234 ", 16, 0, 0);
  test_strtol("- 0x01234 ", 8, 0, 0);
  test_strtol("- 0x01234 ", 10, 0, 0);
  test_strtol("- 0x01234 ", 3, 0, 0);
  test_strtol("- 0x01234 ", 36, 0, 0);

  test_strtol(" -0x01234", 0, 9, -0x1234);
  test_strtol(" -0x01234", 16, 9, -0x1234);
  test_strtol(" -0x01234", 8, 3, 0);
  test_strtol(" -0x01234", 10, 3, 0);
  test_strtol(" -0x01234", 3, 3, 0);
  test_strtol(" -0x01234", 36, 9, -1995433168);

  test_strtol(" -0x01234 ", 0, 9, -0x1234);
  test_strtol(" -0x01234 ", 16, 9, -0x1234);
  test_strtol(" -0x01234 ", 8, 3, 0);
  test_strtol(" -0x01234 ", 10, 3, 0);
  test_strtol(" -0x01234 ", 3, 3, 0);
  test_strtol(" -0x01234 ", 36, 9, -1995433168);

  test_strtol("-0x01234 ", 0, 8, -0x1234);
  test_strtol("-0x01234 ", 16, 8, -0x1234);
  test_strtol("-0x01234 ", 8, 2, 0);
  test_strtol("-0x01234 ", 10, 2, 0);
  test_strtol("-0x01234 ", 3, 2, 0);
  test_strtol("-0x01234 ", 36, 8, -1995433168);

  test_strtol("+0x01234", 0, 8, 0x1234);
  test_strtol("+0x01234", 16, 8, 0x1234);
  test_strtol("+0x01234", 8, 2, 0);
  test_strtol("+0x01234", 10, 2, 0);
  test_strtol("+0x01234", 3, 2, 0);
  test_strtol("+0x01234", 36, 8, 1995433168);

  test_strtol("+ 0x01234", 0, 0, 0);
  test_strtol("+ 0x01234", 16, 0, 0);
  test_strtol("+ 0x01234", 8, 0, 0);
  test_strtol("+ 0x01234", 10, 0, 0);
  test_strtol("+ 0x01234", 3, 0, 0);
  test_strtol("+ 0x01234", 36, 0, 0);

  test_strtol("+ 0x01234 ", 0, 0, 0);
  test_strtol("+ 0x01234 ", 16, 0, 0);
  test_strtol("+ 0x01234 ", 8, 0, 0);
  test_strtol("+ 0x01234 ", 10, 0, 0);
  test_strtol("+ 0x01234 ", 3, 0, 0);
  test_strtol("+ 0x01234 ", 36, 0, 0);

  test_strtol(" +0x01234", 0, 9, 0x1234);
  test_strtol(" +0x01234", 16, 9, 0x1234);
  test_strtol(" +0x01234", 8, 3, 0);
  test_strtol(" +0x01234", 10, 3, 0);
  test_strtol(" +0x01234", 3, 3, 0);
  test_strtol(" +0x01234", 36, 9, 1995433168);

  test_strtol(" +0x01234 ", 0, 9, 0x1234);
  test_strtol(" +0x01234 ", 16, 9, 0x1234);
  test_strtol(" +0x01234 ", 8, 3, 0);
  test_strtol(" +0x01234 ", 10, 3, 0);
  test_strtol(" +0x01234 ", 3, 3, 0);
  test_strtol(" +0x01234 ", 36, 9, 1995433168);

  test_strtol("+0x01234 ", 0, 8, 0x1234);
  test_strtol("+0x01234 ", 16, 8, 0x1234);
  test_strtol("+0x01234 ", 8, 2, 0);
  test_strtol("+0x01234 ", 10, 2, 0);
  test_strtol("+0x01234 ", 3, 2, 0);
  test_strtol("+0x01234 ", 36, 8, 1995433168);

  /* 0X ... */
  test_strtol("0X01234", 0, 7, 0x1234);
  test_strtol("0X01234", 16, 7, 0x1234);
  test_strtol("0X01234", 8, 1, 0);
  test_strtol("0X01234", 10, 1, 0);
  test_strtol("0X01234", 3, 1, 0);
  test_strtol("0X01234", 36, 7, 1995433168);

  /* min/max */
  test_strtol("9223372036854775807", 0, 19, 0x7fffffffffffffffULL);
  test_strtol("+9223372036854775807", 0, 20, 0x7fffffffffffffffULL);
  test_strtol("-9223372036854775808", 0, 20, -0x8000000000000000LL);
  test_strtol("9223372036854775807", 10, 19, 0x7fffffffffffffffULL);
  test_strtol("+9223372036854775807", 10, 20, 0x7fffffffffffffffULL);
  test_strtol("-9223372036854775808", 10, 20, -0x8000000000000000LL);
  test_strtol(
      " +111111111111111111111111111111111111111111111111111111111111111| ", 2,
      65, 0x7fffffffffffffffULL);
  test_strtol(
      " -1000000000000000000000000000000000000000000000000000000000000000| ", 2,
      66, -0x8000000000000000LL);
  test_strtol(" +0x7fffffffffffffff| ", 0, 20, 0x7fffffffffffffffULL);
  test_strtol(" -0x8000000000000000| ", 0, 20, -0x8000000000000000LL);
  test_strtol(" +0x7fffffffffffffff| ", 16, 20, 0x7fffffffffffffffULL);
  test_strtol(" -0x8000000000000000| ", 16, 20, -0x8000000000000000LL);

  /* Overflow/underflow */
  test_strtol("9223372036854775808", 0, 19, 0x7fffffffffffffffULL);
  test_strtol("-9223372036854775809", 0, 20, -0x8000000000000000LL);
  test_strtol("92233720368547758070", 0, 20, 0x7fffffffffffffffULL);
  test_strtol("-92233720368547758080", 0, 21, -0x8000000000000000LL);
}

static void test_strtoul(const char *s, int base, int oexpect,
                         unsigned long int vexpect) {
  static const char *sentinel = "<SENTINEL>";
  unsigned long int tst;
  const char *pexpect = (oexpect != OFFS_SENTINEL) ? (s + oexpect) : sentinel;
  const char *ptst = sentinel;
  int err = 0;

  /* We know that if base is invalid, we should expect the sentinel
   * for endptr, override params */
  if ((base != 0) && ((base < 2) || (base > 36))) {
    pexpect = sentinel;
    vexpect = 0;
  }

  tst = strtoul(s, (char **)&ptst, base);

  if (vexpect != tst) {
    fprintf(stderr, "strtoul('%s', _, %d): retval exp %lu, got %lu\n", s, base,
            vexpect, tst);
    err = 1;
  }

  if (pexpect != ptst) {
    fprintf(stderr, "strtoul('%s', &endptr, %d): endptr exp @%p, got @%p\n", s,
            base, pexpect, ptst);
    fprintf(stderr, "  (endptr exp '%s', got '%s')\n", pexpect, ptst);
    err = 1;
  }

  ASSERT(!err);
}

void strtoul_tests() {
  /* Test invalid convs */
  {
    static const int bases[] = {0, 1, 2, 5, 7, 8, 9, 10, 11, 16, 36, 37};
    unsigned i;
    for (i = 0; i < sizeof(bases) / sizeof(int); ++i) {
      int b = bases[i];

      test_strtoul("", b, 0, 0);
      test_strtoul(" ", b, 0, 0);
      test_strtoul("  ", b, 0, 0);

      test_strtoul("*1234{", b, 0, 0);
      test_strtoul("*01234{", b, 0, 0);
      test_strtoul("*1234{", b, 0, 0);
      test_strtoul("`01234{", b, 0, 0);
      test_strtoul(" {1234", b, 0, 0);
      test_strtoul("|1234[", b, 0, 0);
      test_strtoul("/01234[", b, 0, 0);
      test_strtoul("\"1234[", b, 0, 0);
      test_strtoul("(01234[", b, 0, 0);
      test_strtoul("[1234", b, 0, 0);
      test_strtoul(" *1234{", b, 0, 0);
      test_strtoul(" *01234{", b, 0, 0);
      test_strtoul(" *1234{", b, 0, 0);
      test_strtoul(" `01234{", b, 0, 0);
      test_strtoul("  {1234", b, 0, 0);
      test_strtoul(" |1234[", b, 0, 0);
      test_strtoul(" /01234[", b, 0, 0);
      test_strtoul(" \"1234[", b, 0, 0);
      test_strtoul(" (01234[", b, 0, 0);
      test_strtoul(" [1234", b, 0, 0);
      test_strtoul("*1234{ ", b, 0, 0);
      test_strtoul("*01234{ ", b, 0, 0);
      test_strtoul("*1234{ ", b, 0, 0);
      test_strtoul("`01234{ ", b, 0, 0);
      test_strtoul(" {1234 ", b, 0, 0);
      test_strtoul("|1234[ ", b, 0, 0);
      test_strtoul("/01234[ ", b, 0, 0);
      test_strtoul("\"1234[ ", b, 0, 0);
      test_strtoul("(01234[ ", b, 0, 0);
      test_strtoul("[1234 ", b, 0, 0);
      test_strtoul(" *1234{ ", b, 0, 0);
      test_strtoul(" *01234{ ", b, 0, 0);
      test_strtoul(" *1234{ ", b, 0, 0);
      test_strtoul(" `01234{ ", b, 0, 0);
      test_strtoul("  {1234 ", b, 0, 0);
      test_strtoul(" |1234[ ", b, 0, 0);
      test_strtoul(" /01234[ ", b, 0, 0);
      test_strtoul(" \"1234[ ", b, 0, 0);
      test_strtoul(" (01234[ ", b, 0, 0);
      test_strtoul(" [1234 ", b, 0, 0);

      test_strtoul("", b, 0, 0);
      test_strtoul(" ", b, 0, 0);
      test_strtoul("  ", b, 0, 0);
      test_strtoul("-", b, 0, 0);
      test_strtoul(" -", b, 0, 0);
      test_strtoul("  -", b, 0, 0);
      test_strtoul("- ", b, 0, 0);
      test_strtoul(" - ", b, 0, 0);
      test_strtoul("  - ", b, 0, 0);
      test_strtoul("-  ", b, 0, 0);
      test_strtoul(" -  ", b, 0, 0);
      test_strtoul("  -  ", b, 0, 0);
      test_strtoul("+", b, 0, 0);
      test_strtoul(" +", b, 0, 0);
      test_strtoul("  +", b, 0, 0);
      test_strtoul("+ ", b, 0, 0);
      test_strtoul(" + ", b, 0, 0);
      test_strtoul("  + ", b, 0, 0);
      test_strtoul("+  ", b, 0, 0);
      test_strtoul(" +  ", b, 0, 0);
      test_strtoul("  +  ", b, 0, 0);

      test_strtoul("+|1234{", b, 0, 0);
      test_strtoul(" + )01234{", b, 0, 0);
      test_strtoul("+ +1234{", b, 0, 0);
      test_strtoul(" + -01234{", b, 0, 0);
      test_strtoul(" - -1234", b, 0, 0);
      test_strtoul(" + +1234[", b, 0, 0);
      test_strtoul(" + -01234[", b, 0, 0);
      test_strtoul(" - *1234[", b, 0, 0);
      test_strtoul("-/01234[", b, 0, 0);
      test_strtoul("$[x1234", b, 0, 0);

      test_strtoul("- 0", b, 0, 0);
      test_strtoul("- 0 ", b, 0, 0);
      test_strtoul("+ 0", b, 0, 0);
      test_strtoul("+ 0 ", b, 0, 0);

      /* check 0 in all bases */
      test_strtoul("0", b, 1, 0);
      test_strtoul(" 0", b, 2, 0);
      test_strtoul("0 ", b, 1, 0);
      test_strtoul(" 0 ", b, 2, 0);

      test_strtoul("00", b, 2, 0);
      test_strtoul(" 00", b, 3, 0);
      test_strtoul("00 ", b, 2, 0);
      test_strtoul(" 00 ", b, 3, 0);

      test_strtoul("-0", b, 2, 0);
      test_strtoul(" -0", b, 3, 0);
      test_strtoul("-0 ", b, 2, 0);
      test_strtoul(" -0 ", b, 3, 0);

      test_strtoul("+0", b, 2, 0);
      test_strtoul(" +0", b, 3, 0);
      test_strtoul("+0 ", b, 2, 0);
      test_strtoul(" +0 ", b, 3, 0);
    }
  }

  /* 01234 */
  test_strtoul("01234", 0, 5, 01234);
  test_strtoul("01234", 8, 5, 01234);
  test_strtoul("01234", 10, 5, 1234);
  test_strtoul("01234", 3, 3, 5);
  test_strtoul("01234", 36, 5, 49360);

  test_strtoul(" 01234", 0, 6, 01234);
  test_strtoul(" 01234", 8, 6, 01234);
  test_strtoul(" 01234", 10, 6, 1234);
  test_strtoul(" 01234", 3, 4, 5);
  test_strtoul(" 01234", 36, 6, 49360);

  test_strtoul("01234 ", 0, 5, 01234);
  test_strtoul("01234 ", 8, 5, 01234);
  test_strtoul("01234 ", 10, 5, 1234);
  test_strtoul("01234 ", 3, 3, 5);
  test_strtoul("01234 ", 36, 5, 49360);

  test_strtoul(" 01234 ", 0, 6, 01234);
  test_strtoul(" 01234 ", 8, 6, 01234);
  test_strtoul(" 01234 ", 10, 6, 1234);
  test_strtoul(" 01234 ", 3, 4, 5);
  test_strtoul(" 01234 ", 36, 6, 49360);

  test_strtoul("-01234", 0, 6, -01234);
  test_strtoul("-01234", 8, 6, -01234);
  test_strtoul("-01234", 10, 6, -1234);
  test_strtoul("-01234", 3, 4, -5);
  test_strtoul("-01234", 36, 6, -49360);

  test_strtoul(" -01234", 0, 7, -01234);
  test_strtoul(" -01234", 8, 7, -01234);
  test_strtoul(" -01234", 10, 7, -1234);
  test_strtoul(" -01234", 3, 5, -5);
  test_strtoul(" -01234", 36, 7, -49360);

  test_strtoul("- 01234", 0, 0, 0);
  test_strtoul("- 01234", 8, 0, 0);
  test_strtoul("- 01234", 10, 0, 0);
  test_strtoul("- 01234", 3, 0, 0);
  test_strtoul("- 01234", 36, 0, 0);

  test_strtoul("-01234 ", 0, 6, -01234);
  test_strtoul("-01234 ", 8, 6, -01234);
  test_strtoul("-01234 ", 10, 6, -1234);
  test_strtoul("-01234 ", 3, 4, -5);
  test_strtoul("-01234 ", 36, 6, -49360);

  test_strtoul("- 01234 ", 0, 0, 0);
  test_strtoul("- 01234 ", 8, 0, 0);
  test_strtoul("- 01234 ", 10, 0, 0);
  test_strtoul("- 01234 ", 3, 0, 0);
  test_strtoul("- 01234 ", 36, 0, 0);

  test_strtoul(" -01234 ", 0, 7, -01234);
  test_strtoul(" -01234 ", 8, 7, -01234);
  test_strtoul(" -01234 ", 10, 7, -1234);
  test_strtoul(" -01234 ", 3, 5, -5);
  test_strtoul(" -01234 ", 36, 7, -49360);

  test_strtoul("+01234", 0, 6, 01234);
  test_strtoul("+01234", 8, 6, 01234);
  test_strtoul("+01234", 10, 6, 1234);
  test_strtoul("+01234", 3, 4, 5);
  test_strtoul("+01234", 36, 6, 49360);

  test_strtoul(" +01234", 0, 7, 01234);
  test_strtoul(" +01234", 8, 7, 01234);
  test_strtoul(" +01234", 10, 7, 1234);
  test_strtoul(" +01234", 3, 5, 5);
  test_strtoul(" +01234", 36, 7, 49360);

  test_strtoul("+ 01234", 0, 0, 0);
  test_strtoul("+ 01234", 8, 0, 0);
  test_strtoul("+ 01234", 10, 0, 0);
  test_strtoul("+ 01234", 3, 0, 0);
  test_strtoul("+ 01234", 36, 0, 0);

  test_strtoul("+01234 ", 0, 6, 01234);
  test_strtoul("+01234 ", 8, 6, 01234);
  test_strtoul("+01234 ", 10, 6, 1234);
  test_strtoul("+01234 ", 3, 4, 5);
  test_strtoul("+01234 ", 36, 6, 49360);

  test_strtoul("+ 01234 ", 0, 0, 0);
  test_strtoul("+ 01234 ", 8, 0, 0);
  test_strtoul("+ 01234 ", 10, 0, 0);
  test_strtoul("+ 01234 ", 3, 0, 0);
  test_strtoul("+ 01234 ", 36, 0, 0);

  test_strtoul(" +01234 ", 0, 7, 01234);
  test_strtoul(" +01234 ", 8, 7, 01234);
  test_strtoul(" +01234 ", 10, 7, 1234);
  test_strtoul(" +01234 ", 3, 5, 5);
  test_strtoul(" +01234 ", 36, 7, 49360);

  /* 1234 */
  test_strtoul("1234", 0, 4, 1234);
  test_strtoul("1234", 8, 4, 01234);
  test_strtoul("1234", 10, 4, 1234);
  test_strtoul("1234", 3, 2, 5);
  test_strtoul("1234", 36, 4, 49360);

  test_strtoul(" 1234", 0, 5, 1234);
  test_strtoul(" 1234", 8, 5, 01234);
  test_strtoul(" 1234", 10, 5, 1234);
  test_strtoul(" 1234", 3, 3, 5);
  test_strtoul(" 1234", 36, 5, 49360);

  test_strtoul("1234 ", 0, 4, 1234);
  test_strtoul("1234 ", 8, 4, 01234);
  test_strtoul("1234 ", 10, 4, 1234);
  test_strtoul("1234 ", 3, 2, 5);
  test_strtoul("1234 ", 36, 4, 49360);

  test_strtoul(" 1234 ", 0, 5, 1234);
  test_strtoul(" 1234 ", 8, 5, 01234);
  test_strtoul(" 1234 ", 10, 5, 1234);
  test_strtoul(" 1234 ", 3, 3, 5);
  test_strtoul(" 1234 ", 36, 5, 49360);

  test_strtoul("-1234", 0, 5, -1234);
  test_strtoul("-1234", 8, 5, -01234);
  test_strtoul("-1234", 10, 5, -1234);
  test_strtoul("-1234", 3, 3, -5);
  test_strtoul("-1234", 36, 5, -49360);

  test_strtoul(" -1234", 0, 6, -1234);
  test_strtoul(" -1234", 8, 6, -01234);
  test_strtoul(" -1234", 10, 6, -1234);
  test_strtoul(" -1234", 3, 4, -5);
  test_strtoul(" -1234", 36, 6, -49360);

  test_strtoul("- 1234", 0, 0, 0);
  test_strtoul("- 1234", 8, 0, 0);
  test_strtoul("- 1234", 10, 0, 0);
  test_strtoul("- 1234", 3, 0, 0);
  test_strtoul("- 1234", 36, 0, 0);

  test_strtoul("-1234 ", 0, 5, -1234);
  test_strtoul("-1234 ", 8, 5, -01234);
  test_strtoul("-1234 ", 10, 5, -1234);
  test_strtoul("-1234 ", 3, 3, -5);
  test_strtoul("-1234 ", 36, 5, -49360);

  test_strtoul("- 1234 ", 0, 0, 0);
  test_strtoul("- 1234 ", 8, 0, 0);
  test_strtoul("- 1234 ", 10, 0, 0);
  test_strtoul("- 1234 ", 3, 0, 0);
  test_strtoul("- 1234 ", 36, 0, 0);

  test_strtoul(" -1234 ", 0, 6, -1234);
  test_strtoul(" -1234 ", 8, 6, -01234);
  test_strtoul(" -1234 ", 10, 6, -1234);
  test_strtoul(" -1234 ", 3, 4, -5);
  test_strtoul(" -1234 ", 36, 6, -49360);

  test_strtoul("+1234", 0, 5, 1234);
  test_strtoul("+1234", 8, 5, 01234);
  test_strtoul("+1234", 10, 5, 1234);
  test_strtoul("+1234", 3, 3, 5);
  test_strtoul("+1234", 36, 5, 49360);

  test_strtoul(" +1234", 0, 6, 1234);
  test_strtoul(" +1234", 8, 6, 01234);
  test_strtoul(" +1234", 10, 6, 1234);
  test_strtoul(" +1234", 3, 4, 5);
  test_strtoul(" +1234", 36, 6, 49360);

  test_strtoul("+ 1234", 0, 0, 0);
  test_strtoul("+ 1234", 8, 0, 0);
  test_strtoul("+ 1234", 10, 0, 0);
  test_strtoul("+ 1234", 3, 0, 0);
  test_strtoul("+ 1234", 36, 0, 0);

  test_strtoul("+1234 ", 0, 5, 1234);
  test_strtoul("+1234 ", 8, 5, 01234);
  test_strtoul("+1234 ", 10, 5, 1234);
  test_strtoul("+1234 ", 3, 3, 5);
  test_strtoul("+1234 ", 36, 5, 49360);

  test_strtoul("+ 1234 ", 0, 0, 0);
  test_strtoul("+ 1234 ", 8, 0, 0);
  test_strtoul("+ 1234 ", 10, 0, 0);
  test_strtoul("+ 1234 ", 3, 0, 0);
  test_strtoul("+ 1234 ", 36, 0, 0);

  test_strtoul(" +1234 ", 0, 6, 1234);
  test_strtoul(" +1234 ", 8, 6, 01234);
  test_strtoul(" +1234 ", 10, 6, 1234);
  test_strtoul(" +1234 ", 3, 4, 5);
  test_strtoul(" +1234 ", 36, 6, 49360);

  /* 0x ... */
  test_strtoul("0xabcd", 0, 6, 0xabcd);
  test_strtoul("0Xabcd", 0, 6, 0xabcd);
  test_strtoul("0xABCD", 0, 6, 0xabcd);
  test_strtoul("0XABCD", 0, 6, 0xabcd);
  test_strtoul("0xabcd", 16, 6, 0xabcd);
  test_strtoul("0Xabcd", 16, 6, 0xabcd);
  test_strtoul("0xABCD", 16, 6, 0xabcd);
  test_strtoul("0XABCD", 16, 6, 0xabcd);
  // test_strtoul("0xabcd", 33, 0, 0);  // BUG in eglibc?... allowed!
  // test_strtoul("0Xabcd", 33, 0, 0);  // BUG in eglibc?... allowed!
  // test_strtoul("0xABCD", 33, 0, 0);  // BUG in eglibc?... allowed!
  // test_strtoul("0XABCD", 33, 0, 0);  // BUG in eglibc?... allowed!
  test_strtoul("0xabcd", 34, 6, 44505265);
  test_strtoul("0Xabcd", 34, 6, 44505265);
  test_strtoul("0xABCD", 34, 6, 44505265);
  test_strtoul("0XABCD", 34, 6, 44505265);

  test_strtoul("0x01234", 0, 7, 0x1234);
  test_strtoul("0x01234", 16, 7, 0x1234);
  test_strtoul("0x01234", 8, 1, 0);
  test_strtoul("0x01234", 10, 1, 0);
  test_strtoul("0x01234", 3, 1, 0);
  test_strtoul("0x01234", 36, 7, 1995433168);

  test_strtoul(" 0x01234", 0, 8, 0x1234);
  test_strtoul(" 0x01234", 16, 8, 0x1234);
  test_strtoul(" 0x01234", 8, 2, 0);
  test_strtoul(" 0x01234", 10, 2, 0);
  test_strtoul(" 0x01234", 3, 2, 0);
  test_strtoul(" 0x01234", 36, 8, 1995433168);

  test_strtoul(" 0x01234 ", 0, 8, 0x1234);
  test_strtoul(" 0x01234 ", 16, 8, 0x1234);
  test_strtoul(" 0x01234 ", 8, 2, 0);
  test_strtoul(" 0x01234 ", 10, 2, 0);
  test_strtoul(" 0x01234 ", 3, 2, 0);
  test_strtoul(" 0x01234 ", 36, 8, 1995433168);

  test_strtoul("0x01234 ", 0, 7, 0x1234);
  test_strtoul("0x01234 ", 16, 7, 0x1234);
  test_strtoul("0x01234 ", 8, 1, 0);
  test_strtoul("0x01234 ", 10, 1, 0);
  test_strtoul("0x01234 ", 3, 1, 0);
  test_strtoul("0x01234 ", 36, 7, 1995433168);

  test_strtoul("-0x01234", 0, 8, -0x1234);
  test_strtoul("-0x01234", 16, 8, -0x1234);
  test_strtoul("-0x01234", 8, 2, 0);
  test_strtoul("-0x01234", 10, 2, 0);
  test_strtoul("-0x01234", 3, 2, 0);
  test_strtoul("-0x01234", 36, 8, -1995433168);

  test_strtoul("- 0x01234", 0, 0, 0);
  test_strtoul("- 0x01234", 16, 0, 0);
  test_strtoul("- 0x01234", 8, 0, 0);
  test_strtoul("- 0x01234", 10, 0, 0);
  test_strtoul("- 0x01234", 3, 0, 0);
  test_strtoul("- 0x01234", 36, 0, 0);

  test_strtoul("- 0x01234 ", 0, 0, 0);
  test_strtoul("- 0x01234 ", 16, 0, 0);
  test_strtoul("- 0x01234 ", 8, 0, 0);
  test_strtoul("- 0x01234 ", 10, 0, 0);
  test_strtoul("- 0x01234 ", 3, 0, 0);
  test_strtoul("- 0x01234 ", 36, 0, 0);

  test_strtoul(" -0x01234", 0, 9, -0x1234);
  test_strtoul(" -0x01234", 16, 9, -0x1234);
  test_strtoul(" -0x01234", 8, 3, 0);
  test_strtoul(" -0x01234", 10, 3, 0);
  test_strtoul(" -0x01234", 3, 3, 0);
  test_strtoul(" -0x01234", 36, 9, -1995433168);

  test_strtoul(" -0x01234 ", 0, 9, -0x1234);
  test_strtoul(" -0x01234 ", 16, 9, -0x1234);
  test_strtoul(" -0x01234 ", 8, 3, 0);
  test_strtoul(" -0x01234 ", 10, 3, 0);
  test_strtoul(" -0x01234 ", 3, 3, 0);
  test_strtoul(" -0x01234 ", 36, 9, -1995433168);

  test_strtoul("-0x01234 ", 0, 8, -0x1234);
  test_strtoul("-0x01234 ", 16, 8, -0x1234);
  test_strtoul("-0x01234 ", 8, 2, 0);
  test_strtoul("-0x01234 ", 10, 2, 0);
  test_strtoul("-0x01234 ", 3, 2, 0);
  test_strtoul("-0x01234 ", 36, 8, -1995433168);

  test_strtoul("+0x01234", 0, 8, 0x1234);
  test_strtoul("+0x01234", 16, 8, 0x1234);
  test_strtoul("+0x01234", 8, 2, 0);
  test_strtoul("+0x01234", 10, 2, 0);
  test_strtoul("+0x01234", 3, 2, 0);
  test_strtoul("+0x01234", 36, 8, 1995433168);

  test_strtoul("+ 0x01234", 0, 0, 0);
  test_strtoul("+ 0x01234", 16, 0, 0);
  test_strtoul("+ 0x01234", 8, 0, 0);
  test_strtoul("+ 0x01234", 10, 0, 0);
  test_strtoul("+ 0x01234", 3, 0, 0);
  test_strtoul("+ 0x01234", 36, 0, 0);

  test_strtoul("+ 0x01234 ", 0, 0, 0);
  test_strtoul("+ 0x01234 ", 16, 0, 0);
  test_strtoul("+ 0x01234 ", 8, 0, 0);
  test_strtoul("+ 0x01234 ", 10, 0, 0);
  test_strtoul("+ 0x01234 ", 3, 0, 0);
  test_strtoul("+ 0x01234 ", 36, 0, 0);

  test_strtoul(" +0x01234", 0, 9, 0x1234);
  test_strtoul(" +0x01234", 16, 9, 0x1234);
  test_strtoul(" +0x01234", 8, 3, 0);
  test_strtoul(" +0x01234", 10, 3, 0);
  test_strtoul(" +0x01234", 3, 3, 0);
  test_strtoul(" +0x01234", 36, 9, 1995433168);

  test_strtoul(" +0x01234 ", 0, 9, 0x1234);
  test_strtoul(" +0x01234 ", 16, 9, 0x1234);
  test_strtoul(" +0x01234 ", 8, 3, 0);
  test_strtoul(" +0x01234 ", 10, 3, 0);
  test_strtoul(" +0x01234 ", 3, 3, 0);
  test_strtoul(" +0x01234 ", 36, 9, 1995433168);

  test_strtoul("+0x01234 ", 0, 8, 0x1234);
  test_strtoul("+0x01234 ", 16, 8, 0x1234);
  test_strtoul("+0x01234 ", 8, 2, 0);
  test_strtoul("+0x01234 ", 10, 2, 0);
  test_strtoul("+0x01234 ", 3, 2, 0);
  test_strtoul("+0x01234 ", 36, 8, 1995433168);

  /* 0X ... */
  test_strtoul("0X01234", 0, 7, 0x1234);
  test_strtoul("0X01234", 16, 7, 0x1234);
  test_strtoul("0X01234", 8, 1, 0);
  test_strtoul("0X01234", 10, 1, 0);
  test_strtoul("0X01234", 3, 1, 0);
  test_strtoul("0X01234", 36, 7, 1995433168);

  /* Big signed values */
  test_strtoul("9223372036854775808", 0, 19, 0x8000000000000000ULL);
  test_strtoul("9223372036854775809", 0, 19, 0x8000000000000001ULL);
  test_strtoul("-9223372036854775809", 0, 20, -0x8000000000000001LL);

  /* min/max */
  test_strtoul("9223372036854775807", 0, 19, 0x7fffffffffffffffULL);
  test_strtoul("+9223372036854775807", 0, 20, 0x7fffffffffffffffULL);
  test_strtoul("-9223372036854775808", 0, 20, -0x8000000000000000LL);
  test_strtoul("9223372036854775807", 10, 19, 0x7fffffffffffffffULL);
  test_strtoul("+9223372036854775807", 10, 20, 0x7fffffffffffffffULL);
  test_strtoul("-9223372036854775808", 10, 20, -0x8000000000000000LL);
  /// test_strtoul("
  /// +111111111111111111111111111111111111111111111111111111111111111| ", 2,
  /// 65, 0x7fffffffffffffffULL);
  test_strtoul(
      " -1000000000000000000000000000000000000000000000000000000000000000| ", 2,
      66, -0x8000000000000000LL);
  test_strtoul(" +0x7fffffffffffffff| ", 0, 20, 0x7fffffffffffffffULL);
  test_strtoul(" -0x8000000000000000| ", 0, 20, -0x8000000000000000LL);
  test_strtoul(" +0x7fffffffffffffff| ", 16, 20, 0x7fffffffffffffffULL);
  test_strtoul(" -0x8000000000000000| ", 16, 20, -0x8000000000000000LL);

  /* abs max */
  test_strtoul("18446744073709551614", 0, 20, 0xfffffffffffffffeULL);
  test_strtoul("18446744073709551615", 0, 20, 0xffffffffffffffffULL);
  test_strtoul("+18446744073709551615", 0, 21, 0xffffffffffffffffULL);
  test_strtoul("18446744073709551615", 10, 20, 0xffffffffffffffffULL);
  test_strtoul("+18446744073709551615", 10, 21, 0xffffffffffffffffULL);
  ///  test_strtoul("
  ///  +1111111111111111111111111111111111111111111111111111111111111111| ", 2,
  ///  66, 0xffffffffffffffffULL);
  test_strtoul(" +0xffffffffffffffff| ", 0, 20, 0xffffffffffffffffULL);
  test_strtoul(" +0xffffffffffffffff| ", 16, 20, 0xffffffffffffffffULL);

  /* overflow */
  test_strtoul("18446744073709551616", 0, 20, 0xffffffffffffffffULL);
  test_strtoul("184467440737095516150", 0, 21, 0xffffffffffffffffULL);
}

static void atol_tests() {
  ASSERT(atol("0") == 0);
  ASSERT(atol("+0") == 0);
  ASSERT(atol("-0") == 0);

  ASSERT(atol("1") == 1);
  ASSERT(atol("+1") == 1);
  ASSERT(atol("-1") == -1);

  ASSERT(atol("0123") == 123);
  ASSERT(atol("+0123") == 123);
  ASSERT(atol("-0123") == -123);

  ASSERT(atol("9223372036854775807") == 0x7fffffffffffffffULL);
  ASSERT(atol("+9223372036854775807") == 0x7fffffffffffffffULL);
  ASSERT(atol("-9223372036854775808") == (long long int)-0x8000000000000000LL);

  ASSERT(atol(" 9223372036854775807") == 0x7fffffffffffffffULL);
  ASSERT(atol(" +9223372036854775807") == 0x7fffffffffffffffULL);
  ASSERT(atol(" -9223372036854775808") == (long long int)-0x8000000000000000LL);

  ASSERT(atol("9223372036854775807 ") == 0x7fffffffffffffffULL);
  ASSERT(atol("+9223372036854775807 ") == 0x7fffffffffffffffULL);
  ASSERT(atol("-9223372036854775808 ") == (long long int)-0x8000000000000000LL);

  ASSERT(atol(" 9223372036854775807 ") == 0x7fffffffffffffffULL);
  ASSERT(atol(" +9223372036854775807 ") == 0x7fffffffffffffffULL);
  ASSERT(atol(" -9223372036854775808 ") ==
         (long long int)-0x8000000000000000LL);
}

int main() {
  strtoul_tests();
  strtol_tests();
  atol_tests();
  printf("That's all, folks!\n");
  return 0;
}
