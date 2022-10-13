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

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_STDIO_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_STDIO_H_

#include <stdarg.h>
#include <sys/types.h>
#include <sys/_cdefs.h>

/* Some assume stdio.h defines NULL, etc. And SUSv2 is actually ok with
 * that: "Inclusion of the <stdio.h> header may also make visible all
 * symbols from <stddef.h>." */
#include <stddef.h>

__BEGIN_DECLS

#define EOF (-1)

typedef struct _FILE_DESCR FILE;

extern FILE *stdout;
extern FILE *stderr;

int vsnprintf(char *str, size_t size, const char *format, va_list ap);
int snprintf(char *str, size_t size, const char *format, ...)
    __attribute__((format(printf, 3, 4)));

int vsprintf(char *str, const char *format, va_list ap);
int sprintf(char *str, const char *format, ...)
    __attribute__((format(printf, 2, 3)));

int vfprintf(FILE *stream, const char *format, va_list ap);
int fprintf(FILE *stream, const char *format, ...)
    __attribute__((format(printf, 2, 3)));

int vprintf(const char *format, va_list ap);
int printf(const char *format, ...) __attribute__((format(printf, 1, 2)));

int fputc(int c, FILE *stream);
int fputs(const char *s, FILE *stream);
int putc(int c, FILE *stream);
int putchar(int c);
int puts(const char *s);

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

int fflush(FILE *stream);

int perror(const char *s);

/*
 * Published but functions NOT implemented!
 */
extern FILE *stdin;

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

int scanf(const char *format, ...);
int sscanf(const char *str, const char *format, ...);
FILE *fopen(const char *path, const char *mode);
int fclose(FILE *fp);
char *fgets(char *s, int size, FILE *stream);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);
void rewind(FILE *stream);

__END_DECLS

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_STDIO_H_
