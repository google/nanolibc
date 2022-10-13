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

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_ASSERT_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_ASSERT_H_

#include <sys/_cdefs.h>

__BEGIN_DECLS

void _nnlc_assert(int val, const char *expr, const char *fname, int line,
                  const char *func_name);

__END_DECLS

#define assert(expr)                                        \
  ({                                                        \
    const int __v_assert_val = (int)(expr);                 \
    _nnlc_assert(__v_assert_val, #expr, __FILE__, __LINE__, \
                 __PRETTY_FUNCTION__);                      \
  })

#if __STDC_VERSION__ >= 201112L
# define static_assert _Static_assert
#endif

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_ASSERT_H_
