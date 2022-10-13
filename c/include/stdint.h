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

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_STDINT_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_STDINT_H_

#include <sys/types.h>

#define INT8_C(v)    ((int8_t)v)
#define INT16_C(v)   ((int16_t)v)
#define INT32_C(v)   (v ## L)
#define INT64_C(v)   (v ## LL)
#define UINT8_C(v)   ((uint8_t)v)
#define UINT16_C(v)  ((uint16_t)v)
#define UINT32_C(v)  (v ## UL)
#define UINT64_C(v)  (v ## ULL)
#define SIZET_C(v) ((size_t)v)

// 2^(N-1) - 1
#define INT8_MAX  INT8_C(127)
#define INT16_MAX INT16_C(32767)
#define INT32_MAX INT32_C(2147483647)
#define INT64_MAX INT64_C(9223372036854775807)
#define SIZE_MAX (~SIZET_C(0))

// -2^(N-1)
#define INT8_MIN  (-INT8_MAX - 1)
#define INT16_MIN (-INT16_MAX - 1)
#define INT32_MIN (-INT32_MAX - 1)
#define INT64_MIN (-INT64_MAX - 1)

// 2^(N) - 1
#define UINT8_MAX UINT8_C(255)
#define UINT16_MAX UINT16_C(65535)
#define UINT32_MAX UINT32_C(4294967295)
#define UINT64_MAX UINT64_C(18446744073709551615)

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_STDINT_H_
