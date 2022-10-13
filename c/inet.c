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
 * Implementation of libC arpa/inet functions.
 */
#include <stdio.h>
#include <arpa/inet.h>

const char *inet_ntop(int af, const void *src, char *dst, socklen_t size) {
  if (AF_INET == af) {
    const unsigned char *a = src;            /* in_addr::s_addr */
    if (size < INET_ADDRSTRLEN) return NULL; /* dst too short */

    snprintf(dst, size, "%d.%d.%d.%d", a[0] & 0xff, a[1] & 0xff, a[2] & 0xff,
             a[3] & 0xff);
    return dst;
  }

  /* Unsupported address family */
  return NULL;
}
