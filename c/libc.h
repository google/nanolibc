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
 * Definition of public interface low-level runtime/libC.
 *
 * The libc depends on the implementation of the functions in
 * nnlc_sysdeps structure. The libc is fully operational after
 * _nnlc_initialize() has been called.
 */
#ifndef THIRD_PARTY_NANOLIBC_C_LIBC_H_
#define THIRD_PARTY_NANOLIBC_C_LIBC_H_

#include <stdint.h>
#include <sys/types.h>

/* Hooks to underlying runtime, all function pointers must be defined */
struct nnlc_sysdeps {
  /* the size passed to runtime is guaranteed to be > 0 */
  void *(*malloc)(size_t);

  /* the pointer passed to runtime is guaranteed to be != NULL */
  void (*free)(void *);

  /* runtime must accept ptr == NULL and/or size <= 0 and MUST return
   * number of bytes actually printed. The pointer points to a
   * C-string, bytes with values 0-127 are direct ascii characters
   * otherwise encoding is not specified. */
  ssize_t (*write_stdout)(const void *, size_t);
  ssize_t (*write_stderr)(const void *, size_t);

  /* nanolibc services are still fully available after a call to exit() */
  void (*exit)(int);

  /* return us left to sleep when interrupted */
  uint64_t (*usleep)(uint64_t micro_seconds);

  /* clock_gettime(CLOCK_MONOTONIC, _) implementation. secs/nanosecs
   * passed to runtime are guaranteed to be != NULL. epoch is
   * arbitrary (unspecified). */
  int (*gettime_monotonic)(uint64_t *secs, uint64_t *nanosecs);

  /* clock_gettime(CLOCK_REALTIME, _) implementation. secs/nanosecs
   * passed to runtime are guaranteed to be != NULL. epoch starts at
   * 1970-01-01 00:00:00.0 UTC. */
  int (*gettime_wall)(uint64_t *secs, uint64_t *nanosecs);
};

/* After this function has been called, nanolibc is fully
 * operational. The 'sysdep' param is expected to be non-NULL with all
 * its function pointers defined. This function transfers ownership of
 * 'sysdeps' to nanolibc: it must be valid until the libC is not used
 * anymore (eg. program terminates, etc.). */
int _nnlc_initialize(struct nnlc_sysdeps const *sysdeps);

#endif  // THIRD_PARTY_NANOLIBC_C_LIBC_H_
