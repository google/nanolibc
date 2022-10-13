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
 * Trampolines from native LibC to nanolibC.
 *
 * The idea is:
 *  - we assume we have built our full nanolibc-based executable as a
 *    single big .o fully linked: this contains main() and all
 *    nanolibc functions compiled-in. Only the function pointers to
 *    write/sleep/exit/etc. need to be initialized somehow with a call
 *    to _nnlc_initialize().
 *  - we use ld to rename all the ELF symbols SYM as _NAT2NNL_SYM in
 *    this big .o file
 *  - we use the current C file, compiled as a normal Linux eglibc
 *    executable with a main(), to manually call
 *    _NAT2NNL__nnlc_initialize() to initialize the nanolibc passing
 *    it actual eglibc's functions, and _NAT2NNL_main() to start the
 *    actual nanolibc-based code.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "third_party/nanolibc/c/libc.h"

/* These functions are provided by the big .o file of the application,
 * fully linked */
extern int _NAT2NNL__nnlc_initialize(struct nnlc_sysdeps const *sysdeps);
extern int _NAT2NNL_main(int argc, char *argv[]);

/* We need to correctly define all the pointers in this structure
 * before calling _NAT2NNL__nnlc_initialize(): */
static struct nnlc_sysdeps sd;

#define NNLC_USECS_IN_ONE_SEC (1000000ULL)

static uint64_t nnlc_usleep64(uint64_t micro_seconds) {
  const uint64_t seconds = (micro_seconds / NNLC_USECS_IN_ONE_SEC);
  const uint64_t usecs = micro_seconds % NNLC_USECS_IN_ONE_SEC;

  if (seconds) {
    unsigned int rv = sleep(seconds);
    if (rv > 0) /* interrupted */
      return ((uint64_t)rv) * NNLC_USECS_IN_ONE_SEC;
  }

  if (usecs) usleep(usecs);

  return 0;
}

/* Wrapper for nanolibC to be able to print something using native eglibc */
static ssize_t write_stdout(const void *d, size_t sz) {
  return write(STDOUT_FILENO, d, sz);
}

static ssize_t write_stderr(const void *d, size_t sz) {
  return write(STDERR_FILENO, d, sz);
}

static int nnlc_gettime(clockid_t clid, uint64_t *secs, uint64_t *nanosecs) {
  struct timespec tp;
  int rv;

  rv = clock_gettime(clid, &tp);
  if (0 != rv)
    return rv;

  *secs = tp.tv_sec;
  *nanosecs = tp.tv_nsec;
  return 0;
}

static int gettime_wall(uint64_t *secs, uint64_t *nanosecs) {
  return nnlc_gettime(CLOCK_REALTIME, secs, nanosecs);
}

static int gettime_monotonic(uint64_t *secs, uint64_t *nanosecs) {
  return nnlc_gettime(CLOCK_MONOTONIC, secs, nanosecs);
}

/* native eglibc entry point, initializing nanolibc runtime pointers
 * and calling into the main() in nanolibc executable */
int main(int argc, char *argv[]) {
  int rc;

  sd.malloc = malloc;
  sd.free = free;
  sd.write_stdout = write_stdout;
  sd.write_stderr = write_stderr;
  sd.exit = exit;
  sd.usleep = nnlc_usleep64;
  sd.gettime_wall = gettime_wall;
  sd.gettime_monotonic = gettime_monotonic;

  rc = _NAT2NNL__nnlc_initialize(&sd);
  if (rc) return rc;

  return _NAT2NNL_main(argc, argv);
}

#ifdef COVERAGE
/*
 * Hooks needed when compiled with gcov support: redirected to native gcov lib.
 */
extern void __gcov_init(void *x);
void _NAT2NNL___gcov_init(void *x) { __gcov_init(x); }

extern void __gcov_merge_add(void *x, unsigned u);
void _NAT2NNL___gcov_merge_add(void *x, unsigned u) { __gcov_merge_add(x, u); }
#endif
