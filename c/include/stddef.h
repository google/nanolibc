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

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_STDDEF_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_STDDEF_H_

#include <sys/_cdefs.h>


/* http://pubs.opengroup.org/onlinepubs/7908799/xsh/stddef.h.html */


#ifndef NULL
# ifndef __cplusplus
#  define NULL ((void*)0)
# else
#  define NULL __null
# endif
#endif

#ifndef TRUE
# define TRUE (!0)
#endif

#ifndef FALSE
# define FALSE (!TRUE)
#endif

#define offsetof(st, m) __builtin_offsetof(st, m)

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_STDDEF_H_
