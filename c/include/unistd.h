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

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_UNISTD_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_UNISTD_H_

#include <sys/_cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

unsigned int usleep(unsigned int useconds);
unsigned int sleep(unsigned int seconds);
ssize_t write(int fd, const void *buf, size_t count);

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

/*
 * Published but functions NOT implemented!
 */
int isatty(int fd);
int getopt(int argc, char * const argv[], const char *optstring);
ssize_t read(int fd, void *buf, size_t count);

/*
 * Global variables.
 */
/* for getopt: */
extern char *optarg;
extern int optind, opterr, optopt;

__END_DECLS

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_UNISTD_H_
