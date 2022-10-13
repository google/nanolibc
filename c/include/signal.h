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

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_SIGNAL_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_SIGNAL_H_

#include <sys/_cdefs.h>

#define SIGHUP 1
#define SIGINT 2
#define SIGQUIT 3
#define SIGPIPE 13
#define SIGTERM 15

__BEGIN_DECLS

/*
 * Published but functions NOT implemented!
 */
typedef int sig_atomic_t;
typedef void (*sighandler_t)(int);

sighandler_t signal(int signum, sighandler_t handler);
int raise(int sig);

/* Fake signal functions.  */
#define SIG_ERR ((sighandler_t)-1) /* Error return.  */
#define SIG_DFL ((sighandler_t)0)  /* Default action.  */
#define SIG_IGN ((sighandler_t)1)  /* Ignore signal.  */

__END_DECLS

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_SIGNAL_H_
