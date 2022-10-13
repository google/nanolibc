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

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_LINUX_TYPES_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_LINUX_TYPES_H_

#include <sys/types.h>

typedef uint8_t __u8;
typedef uint16_t __u16;
typedef uint16_t __be16;
typedef uint16_t __le16;
typedef uint32_t __u32;
typedef uint32_t __be32;
typedef uint32_t __le32;
typedef uint64_t __u64;
typedef uint64_t __be64;
typedef uint64_t __le64;

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_LINUX_TYPES_H_
