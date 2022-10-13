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

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_BYTESWAP_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_BYTESWAP_H_

#define __bswap_64 __builtin_bswap64
#define bswap_64 __bswap_64

#define __bswap_32 __builtin_bswap32
#define bswap_32 __bswap_32

#define __bswap_16 __builtin_bswap16
#define bswap_16 __bswap_16

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_BYTESWAP_H_
