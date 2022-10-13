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

/*
 * Definition of private libC structures.
 */
#ifndef THIRD_PARTY_NANOLIBC_C_LIBC_INTERNALS_H_
#define THIRD_PARTY_NANOLIBC_C_LIBC_INTERNALS_H_

#include <sys/types.h>

#include "third_party/nanolibc/c/libc.h"

/* Internal definition of a nanolibc FILE* */
struct _FILE_DESCR {
#define _NNLC_STDIO_MAGIC 0x785789
  int magic;
  ssize_t (*write)(const void *, size_t);
};

/* Internal definition of the nanolibc state */
struct nnlc_internal_data {
  struct _FILE_DESCR libc_stdin, libc_stdout, libc_stderr;
  struct nnlc_sysdeps const *sysdeps;
};
extern struct nnlc_internal_data __nnlc_internal_data;

#endif  // THIRD_PARTY_NANOLIBC_C_LIBC_INTERNALS_H_
