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

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_STDLIB_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_STDLIB_H_

#include <stddef.h>
#include <sys/types.h>
#include <malloc.h> /* malloc/free */

#include <sys/_cdefs.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

__BEGIN_DECLS

long int strtol(const char *nptr, char **endptr, int base);
long long int strtoll(const char *nptr, char **endptr, int base);
unsigned long int strtoul(const char *nptr, char **endptr, int base);
unsigned long long int strtoull(const char *nptr, char **endptr, int base);
long atol(const char *nptr);

float strtof(const char *nptr, char **endptr);
double strtod(const char *nptr, char **endptr);

void exit(int status) __attribute__((noreturn));
void abort(void) __attribute__((noreturn));

/*
 * Published but functions NOT implemented!
 */
char *getenv(const char *name);
void qsort(void *base, size_t nmemb, size_t size,
           int (*compar)(const void *, const void *));

__END_DECLS

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_STDLIB_H_
