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
 * Implementation of libC initialization routines and globals.
 */

#include <stddef.h>

#include "third_party/nanolibc/c/libc_internals.h"

/* nanolibc internal state */
struct nnlc_internal_data __nnlc_internal_data;

/* prepare libc services */
int _nnlc_initialize(struct nnlc_sysdeps const* sysdeps) {
  __nnlc_internal_data.sysdeps = sysdeps;

  __nnlc_internal_data.libc_stdin.magic = _NNLC_STDIO_MAGIC;
  __nnlc_internal_data.libc_stdin.write = NULL;

  __nnlc_internal_data.libc_stdout.magic = _NNLC_STDIO_MAGIC;
  __nnlc_internal_data.libc_stdout.write = sysdeps->write_stdout;

  __nnlc_internal_data.libc_stderr.magic = _NNLC_STDIO_MAGIC;
  __nnlc_internal_data.libc_stderr.write = sysdeps->write_stderr;

  return 0;
}
