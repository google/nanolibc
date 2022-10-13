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

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_SYS_TIME_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_SYS_TIME_H_

#include <sys/_cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

struct timeval {
  time_t      tv_sec;     /* seconds */
  suseconds_t tv_usec;    /* microseconds */
};

int gettimeofday(struct timeval *tv, void *unsupported_tz);

/*
 * _BSD_SOURCE extensions
 *
 * All functions below expect timeval params to be normalized.
 */
#define _GGL_USECS_PER_SEC (1000000L)

inline static void timeradd(struct timeval *a, struct timeval *b,
                            struct timeval *res) {
  res->tv_sec = a->tv_sec + b->tv_sec;
  res->tv_usec = a->tv_usec + b->tv_usec;
  if (res->tv_usec >= _GGL_USECS_PER_SEC) {
    res->tv_sec++;
    res->tv_usec -= _GGL_USECS_PER_SEC;
  }
}

inline static void timersub(struct timeval *a, struct timeval *b,
                            struct timeval *res) {
  res->tv_sec = a->tv_sec - b->tv_sec;
  res->tv_usec = a->tv_usec - b->tv_usec;
  if (res->tv_usec < 0) {
    res->tv_sec--;
    res->tv_usec += _GGL_USECS_PER_SEC;
  }
}

#define timercmp(a, b, CMP)           \
  (((a)->tv_sec == (b)->tv_sec) ?     \
   ((a)->tv_usec CMP (b)->tv_usec) :  \
   ((a)->tv_sec CMP (b)->tv_sec))

__END_DECLS

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_SYS_TIME_H_
