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

/* more string.h tests */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "third_party/nanolibc/tests/test_utils/nnlc_test.h"

static void test_memops() {
  static const char psrc[] = "moufmoufblah";
  static char pdest[sizeof(psrc) + 1];
  unsigned i;

  pdest[0] = 'X';
  pdest[sizeof(psrc)] = 'Z';

  ASSERT(memset(pdest + 1, 0x5a, sizeof(psrc) - 1) == pdest + 1);
  ASSERT(pdest[0] == 'X');
  ASSERT(pdest[sizeof(psrc)] == 'Z');
  for (i = 0; i < sizeof(psrc) - 1; ++i) ASSERT(pdest[i + 1] == 0x5a);

  ASSERT(memcpy(pdest + 1, psrc, sizeof(psrc) - 1) == pdest + 1);
  /* pdest should now be: "XmoufmoufblahZ" */
  ASSERT(pdest[0] == 'X');
  ASSERT(0 < memcmp(pdest, "W", 1));
  ASSERT(0 == memcmp(pdest, "X", 1));
  ASSERT(memcmp(pdest, "Y", 1) < 0);
  ASSERT(0 < memcmp(pdest, "Xl", 2));
  ASSERT(0 == memcmp(pdest, "Xm", 2));
  ASSERT(memcmp(pdest, "Xn", 2) < 0);
  ASSERT(0 == memcmp(pdest + 1, psrc, sizeof(psrc) - 1));
  ASSERT(0 != memcmp(pdest, psrc, sizeof(psrc)));
  ASSERT(pdest[sizeof(psrc)] == 'Z');
  for (i = 0; i < sizeof(psrc) - 1; ++i) ASSERT(pdest[i + 1] == psrc[i]);
  char buf[] = "1234567890";
  memmove(buf + 4, buf, 5);
  ASSERT(!strcmp(buf, "1234123450"));
  memmove(buf, buf + 3, 5);
  ASSERT(!strcmp(buf, "4123423450"));
}

static void test_strops() {
  static const char psrc[] = "moufmoufblah";
  static char pdest[sizeof(psrc) + 3];
  unsigned i;

  pdest[0] = 'X';
  pdest[sizeof(psrc)] = 'Z';
  pdest[sizeof(psrc) + 1] = '!';

  ASSERT(strcpy(pdest + 1, psrc) == pdest + 1);
  ASSERT(pdest[0] == 'X');
  ASSERT(pdest[sizeof(psrc)] == '\0');
  ASSERT(pdest[sizeof(psrc) + 1] == '!');
  for (i = 0; i < sizeof(psrc); ++i) ASSERT(pdest[i + 1] == psrc[i]);

  ASSERT(0 == strcmp(pdest + 1, psrc));
  ASSERT(0 == strcasecmp(pdest + 1, psrc));

  ASSERT(strcmp("", "") == 0);
  ASSERT(strcmp("", "a") < 0);
  ASSERT(strcmp("a", "") > 0);

  ASSERT(strcmp("a", "a") == 0);
  ASSERT(strcmp("a", "b") < 0);
  ASSERT(strcmp("b", "a") > 0);
  ASSERT(strcmp("a", "ab") < 0);
  ASSERT(strcmp("a", "a0") < 0);
  ASSERT(strcmp("ab", "a") > 0);
  ASSERT(strcmp("a0", "a") > 0);

  ASSERT(strcmp("abc", "abc") == 0);
  ASSERT(strcmp("abc", "abd") < 0);
  ASSERT(strcmp("abc", "abb") > 0);

  ASSERT(strcmp("abc", "ab") > 0);
  ASSERT(strcmp("aab", "ab") < 0);
  ASSERT(strcmp("acb", "ab") > 0);

  ASSERT(strcmp("ab", "abc") < 0);
  ASSERT(strcmp("ab", "aab") > 0);
  ASSERT(strcmp("ab", "acb") < 0);

  ASSERT(strncmp("", "", 0) == 0);
  ASSERT(strncmp("", "", 1) == 0);
  ASSERT(strncmp("", "", 10) == 0);
  ASSERT(strncmp("", "a", 0) == 0);
  ASSERT(strncmp("a", "", 0) == 0);
  ASSERT(strncmp("", "a", 1) < 0);
  ASSERT(strncmp("a", "", 1) > 0);
  ASSERT(strncmp("", "a", 10) < 0);
  ASSERT(strncmp("a", "", 10) > 0);

  ASSERT(strncmp("abc", "abc", 0) == 0);
  ASSERT(strncmp("abc", "abc", 1) == 0);
  ASSERT(strncmp("abc", "abc", 2) == 0);
  ASSERT(strncmp("abc", "abd", 3) < 0);
  ASSERT(strncmp("abc", "abb", 3) > 0);
  ASSERT(strncmp("abc", "abd", 10) < 0);
  ASSERT(strncmp("abc", "abb", 10) > 0);

  ASSERT(strcasecmp("", "") == 0);
  ASSERT(strcasecmp("", "a") < 0);
  ASSERT(strcasecmp("a", "") > 0);

  ASSERT(strcasecmp("a", "a") == 0);
  ASSERT(strcasecmp("a", "B") < 0);
  ASSERT(strcasecmp("B", "a") > 0);
  ASSERT(strcasecmp("A", "aB") < 0);
  ASSERT(strcasecmp("A", "a0") < 0);
  ASSERT(strcasecmp("Ab", "a") > 0);
  ASSERT(strcasecmp("a0", "A") > 0);

  ASSERT(strcasecmp("abc", "aBc") == 0);
  ASSERT(strcasecmp("aBc", "abd") < 0);
  ASSERT(strcasecmp("ABC", "aBb") > 0);

  ASSERT(strcasecmp("abC", "Ab") > 0);
  ASSERT(strcasecmp("aAB", "aB") < 0);
  ASSERT(strcasecmp("aCb", "aB") > 0);

  ASSERT(strcasecmp("Ab", "abc") < 0);
  ASSERT(strcasecmp("ab", "AAb") > 0);
  ASSERT(strcasecmp("ab", "aCb") < 0);

  ASSERT(strncasecmp("", "", 0) == 0);
  ASSERT(strncasecmp("", "", 1) == 0);
  ASSERT(strncasecmp("", "", 10) == 0);
  ASSERT(strncasecmp("", "a", 0) == 0);
  ASSERT(strncasecmp("A", "", 0) == 0);
  ASSERT(strncasecmp("", "a", 1) < 0);
  ASSERT(strncasecmp("a", "", 1) > 0);
  ASSERT(strncasecmp("", "A", 10) < 0);
  ASSERT(strncasecmp("a", "", 10) > 0);

  ASSERT(strncasecmp("Abc", "aBc", 0) == 0);
  ASSERT(strncasecmp("aBc", "Abc", 1) == 0);
  ASSERT(strncasecmp("aBc", "abc", 2) == 0);
  ASSERT(strncasecmp("aBc", "abd", 3) < 0);
  ASSERT(strncasecmp("abC", "abb", 3) > 0);
  ASSERT(strncasecmp("abc", "abD", 10) < 0);
  ASSERT(strncasecmp("aBc", "abb", 10) > 0);

  ASSERT(strchr(psrc, 'o') == &psrc[1]);
  ASSERT(strchr(psrc, 'z') == NULL);

  ASSERT(strrchr(psrc, 'o') == &psrc[5]);
  ASSERT(strrchr(psrc, 'z') == NULL);

  ASSERT(strstr(psrc, "") == psrc);
  ASSERT(strstr(psrc, "o") == &psrc[1]);
  ASSERT(strstr(psrc, "fmouf") == &psrc[3]);
  ASSERT(strstr(psrc, "fmoub") == NULL);
}

static void check_strtok(char *s, const char *pat, const char *exp_result) {
  char *r = strtok(s, pat);

  if (exp_result == NULL)
    ASSERT(!r);
  else
    ASSERT(r && !strcmp(r, exp_result));
}

static void test_strtok() {
  char s1[] = "abcdefghi";
  char s2[] = "abcdefghi";

  check_strtok(s1, "z", "abcdefghi");
  check_strtok(NULL, "a", NULL);

  check_strtok(s2, "d", "abc");
  check_strtok(NULL, "e", "fghi");
  check_strtok(NULL, "i", NULL);
}

static void test_memchr() {
  char s1[] = "This is a test.";
  ASSERT(!strcmp(memchr(s1, 'a', sizeof(s1)), "a test."));
  ASSERT(!strcmp(memchr(s1, 't', sizeof(s1)), "test."));
  ASSERT(memchr(s1, 'z', sizeof(s1)) == NULL);
  // Verify chars are treated as unsigned.
  char s2[] = "\xde\xad\xbe\xef";
  // Should match 0xbe.
  ASSERT(!strcmp(memchr(s2, -0x42, sizeof(s1)), "\xbe\xef"));
}

int main() {
  test_memops();
  test_strops();
  test_strtok();
  test_memchr();

  printf("That's all, folks!\n");
  return 0;
}
