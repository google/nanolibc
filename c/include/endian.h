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

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_ENDIAN_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_ENDIAN_H_

/* Compiler expected to define a bunch of macros */
#ifndef __ORDER_LITTLE_ENDIAN__
#define "Non compliant compiler"
#endif
#ifndef __ORDER_BIG_ENDIAN__
#define "Non compliant compiler"
#endif
#ifndef __BYTE_ORDER__
#define "Non compliant compiler"
#endif

/* define standard byte-order macros from compiler's view */
#define __LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__
#define __BIG_ENDIAN __ORDER_BIG_ENDIAN__
#define __PDP_ENDIAN __ORDER_PDP_ENDIAN__

#define __BYTE_ORDER __BYTE_ORDER__

#include <byteswap.h>

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define htobe64(x) __bswap_64(x)
#define htole64(x) (x)
#define be64toh(x) __bswap_64(x)
#define le64toh(x) (x)
#define htobe32(x) __bswap_32(x)
#define htole32(x) (x)
#define be32toh(x) __bswap_32(x)
#define le32toh(x) (x)
#define htobe16(x) __bswap_16(x)
#define htole16(x) (x)
#define be16toh(x) __bswap_16(x)
#define le16toh(x) (x)
#elif __BYTE_ORDER == __BIG_ENDIAN
#define htobe64(x) (x)
#define htole64(x) __bswap_64(x)
#define be64toh(x) (x)
#define le64toh(x) __bswap_64(x)
#define htobe32(x) (x)
#define htole32(x) __bswap_32(x)
#define be32toh(x) (x)
#define le32toh(x) __bswap_32(x)
#define htobe16(x) (x)
#define htole16(x) __bswap_16(x)
#define be16toh(x) (x)
#define le16toh(x) __bswap_16(x)
#else
#error "unsupported endianness"
#endif

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_ENDIAN_H_
