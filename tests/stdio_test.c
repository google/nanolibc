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

/* Test printf() */

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "third_party/nanolibc/tests/test_utils/nnlc_test.h"

#define TPRINTF(expr...)             \
  ({                                 \
    printf("printf(%s) -> ", #expr); \
    printf(expr);                    \
  })

int main() {
  printf("Fun with %%!\n");

  TPRINTF("d1=%016llx d2=%016lx d3=%02x d4=%02X 42=%03d\n",
          (long long unsigned)0xd1, (long unsigned)0xd2, 0xd3, 0xd4, 42);
  TPRINTF("d1=%04x d2=%06x d3=%08x %%100\n", 0xd1, 0xd2, 0xd3);
  TPRINTF("|%-14s| |%-16s| d2=%2x |%-30s|\n", "str14", "str16", 0xd2,
          "12345678901234567890123456789012345");
  TPRINTF("42=%3d d1=%4.4x |%-4s| d2=%8.8x\n", 42, 0xd1, "string4", 0xd2);
  TPRINTF("84=%d 21=%ds |%s| |%sOK| d1=%x d2=%#x\n", 84, 21, "hello", "fine",
          0xd1, 0xd2);

  TPRINTF("%lld\n", -(1ULL << 63));
  TPRINTF("%llx\n", -(1ULL << 63));

  TPRINTF("d1=%.1x\n", 0xd1);
  TPRINTF("d1=%4.1x\n", 0xd1);
  TPRINTF("d1=%4.x\n", 0xd1);

  TPRINTF("1234=%o 00001234=%08o\n", 01234, 01234);
  TPRINTF("a=%c b=%c space='%c' tab='%c' \\1=%c !=%c\n", 'a', 'b', ' ', '\t',
          '\1', '!');

  {
    char s[11];
    int i = snprintf(s, sizeof(s), "Hello |%15s|.", "12345678901234");
    printf("snprintf: result=%d, str='%s' (len=%d)\n", i, s, (int)strlen(s));
    i = snprintf(s, sizeof(s), "Hello |%-15s|.", "12345678901234");
    printf("snprintf: result=%d, str='%s' (len=%d)\n", i, s, (int)strlen(s));
  }

  {
    int in_stack;
    TPRINTF("Adddress of main: %p\n", main);
    TPRINTF("Adddress of stack variable: %p\n", &in_stack);
  }

  fprintf(stdout, "Hello stdout without \\n...");
  fflush(stdout);
  putchar('\n');

  fprintf(stderr, "Hello stderr without \\n...");
  fflush(stderr);
  fputc('\n', stderr);

  puts("puts() to stdout");
  fputs("fputs() to stderr", stderr);
  putc('\n', stderr);

#define FMSG "Test fwrite to stdio\n"
  fwrite(FMSG, sizeof(FMSG) - 1, 1, stdout);
  fwrite(FMSG, sizeof(FMSG) - 1, 1, stderr);
  ASSERT(NULL != strerror(0));

  return 0;
}
