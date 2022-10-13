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

#include <stddef.h>
#include <sys/time.h>
#include <time.h>

#include "third_party/nanolibc/c/libc_internals.h"
#include "third_party/nanolibc/c/nostd/google_warn.h"

time_t time(time_t *t) {
  uint64_t s, ns;
  if (0 != __nnlc_internal_data.sysdeps->gettime_wall(&s, &ns))
    return ((time_t) -1);
  if (NULL != t)
    *t = s;
  return s;
}

int clock_gettime(clockid_t clk_id, struct timespec *tp) {
  uint64_t s, ns;
  int rv = -1;

  if (NULL == tp)
    return rv;

  switch (clk_id) {
    case CLOCK_MONOTONIC:
      rv = __nnlc_internal_data.sysdeps->gettime_monotonic(&s, &ns);
      break;
    case CLOCK_REALTIME:
      rv = __nnlc_internal_data.sysdeps->gettime_wall(&s, &ns);
      break;
    default:
      GGL_WARN_ONCE("only CLOCK_MONOTONIC|REALTIME supported for now.");
      break;
  }

  if (0 != rv)
    return rv;

  tp->tv_sec = s;
  tp->tv_nsec = ns;
  return 0;
}

int gettimeofday(struct timeval *tv, void *unsupported_tz) {
  uint64_t s, ns;
  int rv = __nnlc_internal_data.sysdeps->gettime_wall(&s, &ns);
  if (0 != rv)
    return rv;

  if (NULL != tv) {
    tv->tv_sec = s;
    tv->tv_usec = ns/1000L;
  }

  if (NULL != unsupported_tz) {
    GGL_WARN_ONCE("no support for time zone arg");
    return -1;
  }

  return 0;
}
