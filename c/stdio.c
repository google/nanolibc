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
 * Implementation of stdio.h functions.
 */

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "third_party/nanolibc/c/libc_internals.h"
#include "third_party/tinyprintf/tinyprintf.h"

FILE *stdin = &__nnlc_internal_data.libc_stdin;
FILE *stdout = &__nnlc_internal_data.libc_stdout;
FILE *stderr = &__nnlc_internal_data.libc_stderr;

/* returns TRUE if param 'stream' is a valid FILE* pointer */
inline static int _is_valid_FILE(FILE *stream) {
  return ((stream != NULL) && (stream->magic == _NNLC_STDIO_MAGIC));
}

int fputc(int c, FILE *stream) {
  const unsigned char uc = c; /* according to man */
  if (!_is_valid_FILE(stream) || !stream->write) return EOF;
  if (1 != stream->write(&uc, 1)) return EOF;
  return uc; /* according to man */
}

int fputs(const char *s, FILE *stream) {
  size_t len;
  if (!_is_valid_FILE(stream) || !stream->write) return EOF;
  len = strlen(s);
  if (stream->write(s, len) != (ssize_t)len) return EOF;
  return 1;
}

int putc(int c, FILE *stream) { return fputc(c, stream); }

int putchar(int c) {
  const unsigned char uc = c; /* according to man */
  if (1 != stdout->write(&uc, 1)) return EOF;
  return uc; /* according to man */
}

int puts(const char *s) {
  const size_t len = strlen(s);
  if (stdout->write(s, len) != (ssize_t)len) return EOF;
  if (1 != stdout->write("\n", 1)) return EOF;
  return 1;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
  size_t i;
  if (!_is_valid_FILE(stream) || !stream->write) return 0;
  for (i = 0; i < nmemb; ++i) {
    const void *p = ptr + (i * size);
    if (stream->write(p, size) != (ssize_t)size) break;
  }
  return i;
}

int fflush(FILE *stream) {
  if (!_is_valid_FILE(stream)) return -1;

  return 0;
}

int perror(const char *s) {
  fprintf(stderr, "Error: %s\n", s);
  return 0;
}

/*
 * vprintf family of functions. Rely on tfp_format, provide required
 * callbacks and data for it to call the underlying runtime 'write'
 * functions. We use a small buffer on the stack to avoid calling the
 * underlying 'write' too often.
 */
struct _vprintf_putcf_data {
  FILE *stream;

  char *buff; /* allocated on the stack by vprintf */
  size_t buff_capacity;
  size_t nbuffered; /* slightly redundant with buff_capacity, we don't care */

  size_t total_written;
};

/* Callback function for tfp_format to add the given char c to the buffer */
static void _vprintf_putcf(void *p, char c) {
  struct _vprintf_putcf_data *data = (struct _vprintf_putcf_data *)p;

  /* sanity check */
  if (data->nbuffered >= data->buff_capacity)
    abort(); /* no assert(), to prevent infinite recursion */

  data->buff[data->nbuffered++] = c;

  /* need to flush buffer? */
  if (data->nbuffered >= data->buff_capacity) {
    int n = data->stream->write(data->buff, data->nbuffered);
    if (n > 0) data->total_written += n;
    data->nbuffered = 0;
  }
}

int vfprintf(FILE *stream, const char *format, va_list ap) {
  char buff[64]; /* any size >= 1 is Ok */
  struct _vprintf_putcf_data data;

  if (!_is_valid_FILE(stream) || !stream->write) return -1;

  data.stream = stream;
  data.buff_capacity = sizeof(buff);
  data.buff = buff;
  data.nbuffered = 0;
  data.total_written = 0;
  tfp_format(&data, _vprintf_putcf, format, ap);

  /* chars remaining in buffer? */
  if (data.nbuffered > 0) {
    int n = stream->write(data.buff, data.nbuffered);
    if (n > 0) data.total_written += n;
  }

  return data.total_written;
}

int fprintf(FILE *stream, const char *format, ...) {
  va_list ap;
  int retval;

  va_start(ap, format);
  retval = vfprintf(stream, format, ap);
  va_end(ap);
  return retval;
}

int vprintf(const char *format, va_list ap) {
  return vfprintf(stdout, format, ap);
}

int printf(const char *format, ...) {
  va_list ap;
  int retval;

  va_start(ap, format);
  retval = vprintf(format, ap);
  va_end(ap);

  return retval;
}

/*
 * vsnprintf family of functions. Rely on tfp_format, provide required
 * callbacks and data for it to fill the buffer provided by caller.
 */
struct _vsnprintf_putcf_data {
  size_t dest_capacity;
  char *dest;
  size_t dest_used;
};

static void _vsnprintf_putcf(void *p, char c) {
  struct _vsnprintf_putcf_data *data = (struct _vsnprintf_putcf_data *)p;
  if (data->dest_used < data->dest_capacity) data->dest[data->dest_used] = c;

  data->dest_used++;
}

int vsnprintf(char *str, size_t size, const char *format, va_list ap) {
  struct _vsnprintf_putcf_data data;

  data.dest = str;
  data.dest_capacity = size;
  data.dest_used = 0;
  tfp_format(&data, _vsnprintf_putcf, format, ap);

  if (data.dest_used < size)
    data.dest[data.dest_used] = '\0';
  else if (size > 0)
    data.dest[size - 1] = '\0';

  return data.dest_used;
}

int snprintf(char *str, size_t size, const char *format, ...) {
  va_list ap;
  int retval;

  va_start(ap, format);
  retval = vsnprintf(str, size, format, ap);
  va_end(ap);
  return retval;
}

/*
 * vsprintf family of functions. Rely on tfp_format, provide required
 * callbacks and data for it to fill the buffer provided by caller.
 */
struct _vsprintf_putcf_data {
  char *dest;
  size_t dest_used;
};

static void _vsprintf_putcf(void *p, char c) {
  struct _vsprintf_putcf_data *data = (struct _vsprintf_putcf_data *)p;
  data->dest[data->dest_used++] = c;
}

int vsprintf(char *str, const char *format, va_list ap) {
  struct _vsprintf_putcf_data data;
  data.dest = str;
  data.dest_used = 0;
  tfp_format(&data, _vsprintf_putcf, format, ap);
  data.dest[data.dest_used] = '\0';
  return data.dest_used;
}

int sprintf(char *str, const char *format, ...) {
  va_list ap;
  int retval;

  va_start(ap, format);
  retval = vsprintf(str, format, ap);
  va_end(ap);
  return retval;
}
