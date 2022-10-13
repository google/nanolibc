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

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_LIMITS_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_LIMITS_H_

/* Assuming 64b long words */
#if __SIZEOF_LONG__ != 8
#error "Only target with 64b long supported"
#endif

#define LONG_MAX 9223372036854775807L
#define LONG_MIN (-LONG_MAX - 1L)
#define ULONG_MAX 18446744073709551615UL

#define LLONG_MAX 9223372036854775807LL
#define LLONG_MIN (-LLONG_MAX - 1LL)
#define ULLONG_MAX 18446744073709551615ULL

#define INT_MAX 0x7fffffff
#define INT_MIN 0x80000000
#define UINT_MAX 0xffffffff

#define CHAR_BIT 8

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_LIMITS_H_
