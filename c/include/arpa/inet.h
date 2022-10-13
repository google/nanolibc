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

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_ARPA_INET_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_ARPA_INET_H_

#include <stddef.h>
#include <netinet/in.h>

__BEGIN_DECLS

int inet_aton(const char *cp, struct in_addr *inp);

/* inet_ntoa intentionally NOT implemented */
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
int inet_pton(int af, const char *src, void *dst);

__END_DECLS

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_ARPA_INET_H_
