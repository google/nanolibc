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
 * malloc/free trampolines to underlying runtime.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <malloc.h>

#include "third_party/nanolibc/c/libc_internals.h"

/* Little malloc header required for realloc (memcpy)

   To deal with this, dynamically align the data to 16 bytes regardless of the
   alignment returned by the underlying malloc implementation.  */
struct malloc_block {
  void *mem;  /* The pointer returned by underlying malloc. */
  size_t nbytes;
  char data[0];
};

#define NNLC_ALIGNMENT_MASK (NNLC_MALLOC_ALIGNMENT - 1)

/* Round the requested size up to include enough for alignment. */
static inline intptr_t pad_for_alignment(intptr_t requested_size) {
  return requested_size + NNLC_MALLOC_ALIGNMENT - 1;
}

/* Return a pointer aligned to NNLC_MALLOC_ALIGNMENT at or after `reserved_space`
 * space. */
static void *align_after(void *mem, size_t reserved_space) {
  /* Skip past `reserved_space`. */
  char *p = (char*)mem + reserved_space;
  return (void*)(pad_for_alignment((intptr_t)p) & ~(intptr_t)NNLC_ALIGNMENT_MASK);
}

/* Return a struct malloc_block* pointing to the p - sizeof(struct
   malloc_block). */
static struct malloc_block *get_malloc_block(void *p) {
  return (struct malloc_block*)p - 1;
}

void *malloc(size_t size) {
  struct malloc_block *m;
  void *mem;
  size_t mem_size = pad_for_alignment(size) + sizeof(*m);
  void *p;

  if (size <= 0) return NULL;

  mem = __nnlc_internal_data.sysdeps->malloc(mem_size);
  if (NULL == mem) return NULL;

  p = align_after(mem, sizeof(*m));
  m = get_malloc_block(p);

  m->mem = mem;
  m->nbytes = size;
  return p;
}

void free(void *ptr) {
  if (NULL == ptr) return;

  __nnlc_internal_data.sysdeps->free(get_malloc_block(ptr)->mem);
}

void *calloc(size_t num, size_t size) {
  // Be careful to check for overflow.
  size_t len = num * size;
  if (size == 0 || num != len / size) {
    return NULL;
  }
  void* buf = malloc(len);
  memset(buf, 0, len);
  return buf;
}

void *realloc(void *ptr, size_t size) {
  void *new_ptr;
  size_t prev_size;

  if (NULL == ptr)
    return malloc(size); /* Ok if size <= 0: returns NULL */
  else if (size <= 0) {
    free(ptr);
    return NULL;
  }

  /* Invariants: ptr != NULL and size > 0 */

  prev_size = get_malloc_block(ptr)->nbytes;
  if (size <= prev_size) return ptr; /* no need to change anything */

  /* Invariants: ptr != NULL and size > prev_size > 0 */

  new_ptr = malloc(size);
  if (NULL != new_ptr) memcpy(new_ptr, ptr, prev_size);

  free(ptr);
  return new_ptr;
}
