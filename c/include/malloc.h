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

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_MALLOC_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_MALLOC_H_

#include <sys/types.h>

#include <sys/_cdefs.h>

__BEGIN_DECLS

#define NNLC_MALLOC

/* Clang generates assembly code assuming malloc returns 16-byte aligned data.
   Some move instructions involving xmm registers will segv when it is not.
   This is not technically a compiler bug, because the Clang compiler is only
   designed to work with its own libc/stdlibc++ library.
   NOTE: must be power of 2. */
#define NNLC_MALLOC_ALIGNMENT 16

void *malloc(size_t size);
void *calloc(size_t num, size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);

__END_DECLS

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_MALLOC_H_
