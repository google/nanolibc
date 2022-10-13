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

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_NETINET_IN_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_NETINET_IN_H_

#include <stddef.h>
#include <stdint.h>
#include <endian.h>

/* Deviation from POSIX: supposed to be in sys/socket.h */
#define PF_INET      2 /* Stay compatible with Linux constants */
#define PF_INET6    10 /* Stay compatible with Linux constants */
#define AF_INET     PF_INET
#define AF_INET6    PF_INET6

typedef int socklen_t;
/* End POSIX deviation */

#define INET_ADDRSTRLEN  16
#define INET6_ADDRSTRLEN 46

#define ntohl be32toh
#define htonl htobe32

__BEGIN_DECLS

typedef uint32_t in_addr_t;

struct in_addr {
  in_addr_t s_addr;
};

struct in6_addr {
  union {
    uint8_t s6_addr[16];  /* SUSv2 */
    uint32_t s6_addr32[4];  /* GNU libc (?) */
  };  /* anonymous */
};

__END_DECLS

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_NETINET_IN_H_
