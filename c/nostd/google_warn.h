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

#ifndef THIRD_PARTY_NANOLIBC_C_NOSTD_GOOGLE_WARN_H_
#define THIRD_PARTY_NANOLIBC_C_NOSTD_GOOGLE_WARN_H_

#include <stdio.h>

static void _ggl_warn_once(int *warned, const char *fctname, unsigned line,
                           const char *msg) {
  if (!*warned)
    fprintf(stderr, "WARNING[%s (line %u)]: %s\n", fctname, line, msg);
  *warned = 1;
}

#define _GGL_WARN_ONCE(fctname, line, msg)              \
  ({                                                    \
    static int _warned = 0;                             \
    _ggl_warn_once(&_warned, fctname, line, msg);       \
  })

#define GGL_WARN_ONCE(msg)                              \
  _GGL_WARN_ONCE(__PRETTY_FUNCTION__, __LINE__, msg)

/* Print "unsupported" warning the first time the parent function is called. */
#define GGL_WARN_UNSUPPORTED()                  \
  GGL_WARN_ONCE("Unsupported function called.")

#endif  // THIRD_PARTY_NANOLIBC_C_NOSTD_GOOGLE_WARN_H_
