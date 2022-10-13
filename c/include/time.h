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

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_TIME_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_TIME_H_

#include <sys/_cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

struct tm {
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
  long int tm_gmtoff;
  const char *tm_zone;
};

struct timespec {
  time_t   tv_sec;        /* seconds */
  long     tv_nsec;       /* nanoseconds */
};

#define CLOCK_REALTIME  0
#define TIMER_ABSTIME   1
#define CLOCK_MONOTONIC 1

time_t time(time_t *t);
int clock_gettime(clockid_t clk_id, struct timespec *tp);

/*
 * Published but functions NOT implemented!
 */
struct tm *localtime(const time_t *timep);

// Not implemented.
unsigned int clock(void);

__END_DECLS

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_TIME_H_
