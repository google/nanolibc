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
 * Implementation of libC strings.h functions.
 */

#include <strings.h>
#include <ctype.h>

int strcasecmp(const char *p1, const char *p2) {
  const unsigned char *s1 = (const unsigned char *)p1;
  const unsigned char *s2 = (const unsigned char *)p2;

  while (1) {
    const unsigned int c1 = tolower(*s1++);
    const unsigned int c2 = tolower(*s2++);
    if ((c1 == '\0') || (c1 != c2)) return (c1 - c2);
  }

  /* not reached */
  return 0;
}

int strncasecmp(const char *p1, const char *p2, size_t n) {
  const unsigned char *s1 = (const unsigned char *)p1;
  const unsigned char *s2 = (const unsigned char *)p2;

  while (n > 0) {
    const unsigned char c1 = tolower(*s1++);
    const unsigned char c2 = tolower(*s2++);
    if ((c1 == '\0') || (c1 != c2)) return (c1 - c2);
    n--;
  }

  return 0;
}
