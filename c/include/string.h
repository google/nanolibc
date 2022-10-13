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

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_STRING_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_STRING_H_

#include <sys/_cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

int memcmp(const void *s1, const void *s2, size_t n);
void *memcpy(void *dstpp, const void *srcpp, size_t n);
void *memmove(void *dstpp, const void *srcpp, size_t n);
void *memset(void *dstpp, int c, size_t n);
void *memchr(const void *spp, int c, size_t n);
const void *rawmemchr(const void *spp, int c);

int strcmp(const char *p1, const char *p2);
char *strcpy(char *dest, const char *src);
size_t strlen(const char *s);
int strncmp(const char *s1, const char *s2, size_t n);
char *strncpy(char *dest, const char *src, size_t n);
char *strchr(const char *s, int i);
char *strrchr(const char *s, int i);
const char *strstr(const char *haystack, const char *needle);
size_t strspn(const char *s, const char *accept);
const char *strpbrk(const char *s, const char *accept);

// These two are unsupported.
char *strtok_r(char *str, const char *delim, char **saveptr);
char *strtok(char *str, const char *delim);

char *strdup(const char *s);
char *strndup(const char *s, size_t n);

char *strerror(int errnum);

__END_DECLS

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_STRING_H_
