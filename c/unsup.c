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
 * Published but unsupported functions.
 */

#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>

#include "third_party/nanolibc/c/nostd/google_warn.h"

/*
 * The following define common symbols that are not yet supported
 */

char *getenv(const char *name) {
  (void)name; /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
  return NULL;
}

void qsort(void *base, size_t nmemb, size_t size,
           int (*compar)(const void *, const void *)) {
  (void)base;   /* silence gcc warning */
  (void)nmemb;  /* silence gcc warning */
  (void)size;   /* silence gcc warning */
  (void)compar; /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
  errno = EINVAL;
}

sighandler_t signal(int signum, sighandler_t handler) {
  (void)signum;  /* silence gcc warning */
  (void)handler; /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
  errno = EINVAL;
  return NULL;
}

int raise(int sig) {
  (void)sig; /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
  return -1;
}

FILE *fopen(const char *path, const char *mode) {
  (void)path; /* silence gcc warning */
  (void)mode; /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
  errno = ENOENT;
  return NULL;
}

int fclose(FILE *fp) {
  (void)fp; /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
  errno = EBADF;
  return -1;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
  (void)ptr;    /* silence gcc warning */
  (void)size;   /* silence gcc warning */
  (void)nmemb;  /* silence gcc warning */
  (void)stream; /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
  return 0;
}

int fseek(FILE *stream, long offset, int whence) {
  (void)stream; /* silence gcc warning */
  (void)offset; /* silence gcc warning */
  (void)whence; /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
  errno = EBADF;
  return -1;
}

long ftell(FILE *stream) {
  (void)stream; /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
  errno = EBADF;
  return -1;
}

void rewind(FILE *stream) {
  (void)stream; /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
}

int sscanf(const char *str, const char *format, ...) {
  (void)str;    /* silence gcc warning */
  (void)format; /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
  return -1;
}

int scanf(const char *format, ...) {
  (void)format; /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
  errno = EBADF;
  return EOF;
}

float strtof(const char *nptr, char **endptr) {
  (void)nptr;   /* silence gcc warning */
  (void)endptr; /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
  errno = EOPNOTSUPP;
  return 0;
}

double strtod(const char *nptr, char **endptr) {
  (void)nptr;   /* silence gcc warning */
  (void)endptr; /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
  errno = EOPNOTSUPP;
  return 0;
}

char *fgets(char *s, int size, FILE *stream) {
  (void)s;      /* silence gcc warning */
  (void)size;   /* silence gcc warning */
  (void)stream; /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
  return NULL;
}

struct tm *localtime(const time_t *timep) {
  (void)timep; /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
  return NULL;
}

int isatty(int fd) {
  (void)fd; /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
  errno = EBADF;
  return 0;
}

/* pretend getopt() parsed all args, but actually do nothing */
char *optarg = NULL;
int optind = 1;
int opterr = 1;
int optopt = '?';
int getopt(int argc, char * const argv[], const char *optstring) {
  (void)argc; /* silence gcc warning */
  (void)argv; /* silence gcc warning */
  (void)optstring; /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
  optarg = NULL;
  optind = 1;
  optopt = '?';
  return -1;
}

int inet_aton(const char *cp, struct in_addr *inp) {
  (void)cp;  /* silence gcc warning */
  (void)inp; /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
  return -1;
}

int inet_pton(int af, const char *src, void *dst) {
  (void)af;   /* silence gcc warning */
  (void)src;  /* silence gcc warning */
  (void)dst;  /* silence gcc warning */

  GGL_WARN_UNSUPPORTED();
  return -1;
}

float ceilf(float value) {
  (void)value;  /* silence gcc warning */
  GGL_WARN_UNSUPPORTED();
  return NAN;
}

double ceil(double value) {
  (void)value;  /* silence gcc warning */
  GGL_WARN_UNSUPPORTED();
  return NAN;
}

float floorf(float value) {
  (void)value;  /* silence gcc warning */
  GGL_WARN_UNSUPPORTED();
  return NAN;
}

double floor(double value) {
  (void)value;  /* silence gcc warning */
  GGL_WARN_UNSUPPORTED();
  return NAN;
}

float truncf(float value) {
  (void)value;  /* silence gcc warning */
  GGL_WARN_UNSUPPORTED();
  return NAN;
}

double trunc(double value) {
  (void)value;  /* silence gcc warning */
  GGL_WARN_UNSUPPORTED();
  return NAN;
}

float sqrtf(float value) {
  (void)value;  /* silence gcc warning */
  GGL_WARN_UNSUPPORTED();
  return NAN;
}

double sqrt(double value) {
  (void)value;  /* silence gcc warning */
  GGL_WARN_UNSUPPORTED();
  return NAN;
}

float rintf(float value) {
  (void)value;  /* silence gcc warning */
  GGL_WARN_UNSUPPORTED();
  return NAN;
}

double rint(double value) {
  (void)value;  /* silence gcc warning */
  GGL_WARN_UNSUPPORTED();
  return NAN;
}

unsigned int clock(void) {
  GGL_WARN_UNSUPPORTED();
  return 0;
}

ssize_t read(int fd, void *buf, size_t count) {
  (void)fd;  /* silence gcc warning */
  (void)buf;  /* silence gcc warning */
  (void)count;  /* silence gcc warning */
  GGL_WARN_UNSUPPORTED();
  return 0;
}
