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

/* Test sXprintf() */

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>

#include "third_party/nanolibc/tests/test_utils/nnlc_test.h"

int main() {
  static char buff[1024]; /* expected to be > 12 and > max result
                           * length for the tests */
  ASSERT(sizeof(buff) > 12);

#define TSXPRINTF(expected, fmt, args...)                           \
  ({                                                                \
    const int nexp = strlen(expected);                              \
    char short_exp[12];                                             \
    int got;                                                        \
                                                                    \
    strncpy(short_exp, expected, 11);                               \
    short_exp[11] = '\0';                                           \
                                                                    \
    got = snprintf(buff, sizeof(buff), fmt, args);                  \
    if (strcmp(buff, expected))                                     \
      fprintf(stderr, "expected='%s', got='%s'\n", expected, buff); \
    ASSERT(!strcmp(buff, expected));                                \
    ASSERT(nexp == got);                                            \
                                                                    \
    got = sprintf(buff, fmt, args);                                 \
    ASSERT(!strcmp(buff, expected));                                \
    ASSERT(nexp == got);                                            \
                                                                    \
    got = snprintf(buff, nexp + 1, fmt, args);                      \
    ASSERT(!strcmp(buff, expected));                                \
    ASSERT(nexp == got);                                            \
                                                                    \
    got = snprintf(buff, 0, fmt, args);                             \
    ASSERT(nexp == got);                                            \
                                                                    \
    got = snprintf(buff, 1, fmt, args);                             \
    ASSERT(buff[0] == '\0');                                        \
    ASSERT(nexp == got);                                            \
                                                                    \
    got = snprintf(buff, 12, fmt, args);                            \
    ASSERT(!strcmp(buff, short_exp));                               \
    ASSERT(nexp == got);                                            \
  })

  TSXPRINTF("d1=00000000000000d1 d2=00000000000000d2 d3=d3 d4=D4 42=042\n",
            "d1=%016llx d2=%016lx d3=%02x d4=%02X 42=%03d\n",
            (long long unsigned)0xd1, (long unsigned)0xd2, 0xd3, 0xd4, 42);
  TSXPRINTF("d1=00d1 d2=0000d2 d3=000000d3 %100\n",
            "d1=%04x d2=%06x d3=%08x %%100\n", 0xd1, 0xd2, 0xd3);
  TSXPRINTF(
      "|str14         | |str16           "
      "| d2=d2 |12345678901234567890123456789012345|\n",
      "|%-14s| |%-16s| d2=%2x |%-30s|\n", "str14", "str16", 0xd2,
      "12345678901234567890123456789012345");
  TSXPRINTF("42= 42 d1=00d1 |string4| d2=000000d2\n",
            "42=%3d d1=%4.4x |%-4s| d2=%8.8x\n", 42, 0xd1, "string4", 0xd2);
  TSXPRINTF("84=84 21=21s |hello| |fineOK| d1=d1 d2=0xd2\n",
            "84=%d 21=%ds |%s| |%sOK| d1=%x d2=%#x\n", 84, 21, "hello", "fine",
            0xd1, 0xd2);

  TSXPRINTF("-9223372036854775808\n", "%lld\n", -(1ULL << 63));
  TSXPRINTF("8000000000000000\n", "%llx\n", -(1ULL << 63));

  TSXPRINTF("1234=1234 00001234=00001234\n", "1234=%o 00001234=%08o\n", 01234,
            01234);
  TSXPRINTF("a=a b=b space=' ' tab='\t' \\1=\1 !=!\n",
            "a=%c b=%c space='%c' tab='%c' \\1=%c !=%c\n", 'a', 'b', ' ', '\t',
            '\1', '!');

  {
    size_t some_size = 0;
    TSXPRINTF("d1=0 d2=0 d3=0x0\n", "d1=%zu d2=%zd d3=0x%zx\n", some_size,
              some_size, some_size);
    some_size = 42;
    TSXPRINTF("d1=42 d2=42 d3=0x2a\n", "d1=%zu d2=%zd d3=0x%zx\n", some_size,
              some_size, some_size);
    some_size = ~0;
    if (sizeof(size_t) == 8)
      TSXPRINTF("d1=18446744073709551615 d2=-1 d3=0xffffffffffffffff\n",
                "d1=%zu d2=%zd d3=0x%zx\n", some_size, some_size, some_size);
  }

  TSXPRINTF("true=1 false=0\n", "true=%d false=%d\n", true, false);

  /* nanolib supports only %a.bx formats, with a == b */
  TSXPRINTF("d1=00d1\n", "d1=%4.4x\n", 0xd1);

#define TINETNTOP(x, y, z, t)                                             \
  ({                                                                      \
    struct in_addr ina;                                                   \
    char buff[INET_ADDRSTRLEN];                                           \
    ina.s_addr = htonl((x) << 24 | (y) << 16 | (z) << 8 | (t));           \
    ASSERT(!strcmp(inet_ntop(AF_INET, &ina, buff, sizeof(buff)) ?: "ERR", \
                   #x "." #y "." #z "." #t));                             \
  })

  TINETNTOP(0, 0, 0, 0);
  TINETNTOP(1, 0, 0, 0);
  TINETNTOP(1, 2, 0, 0);
  TINETNTOP(1, 2, 3, 0);
  TINETNTOP(1, 2, 3, 4);
  TINETNTOP(11, 0, 0, 0);
  TINETNTOP(11, 22, 0, 0);
  TINETNTOP(11, 22, 33, 0);
  TINETNTOP(11, 22, 33, 44);
  TINETNTOP(252, 0, 0, 0);
  TINETNTOP(252, 253, 0, 0);
  TINETNTOP(252, 253, 254, 0);
  TINETNTOP(252, 253, 254, 255);
  TINETNTOP(255, 255, 255, 255);

  printf("That's all, folks!\n");
  return 0;
}
