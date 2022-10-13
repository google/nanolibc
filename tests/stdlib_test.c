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

/* test stdlib.h/unistd.h */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "third_party/nanolibc/tests/test_utils/nnlc_test.h"

#if defined(NNLC_MALLOC)
  #define ALIGNMENT NNLC_MALLOC_ALIGNMENT
#elif defined(__clang__) && defined(__x86_64)
  #define ALIGNMENT ((size_t)16)
#else
  #define ALIGNMENT ((size_t)1)
#endif

static void test_malloc_free() {
  char *p;
  int i;

  printf("Starting malloc/free tests...\n");

  free(NULL); /* should not crash */

  p = malloc(0);
  /* value of p is undefined */
  if (p != NULL) free(p);

  p = malloc(1);
  ASSERT(p != NULL);
  ASSERT(((intptr_t)p & (ALIGNMENT - 1)) == 0);
  *p = 'X';
  ASSERT(*p == 'X');
  free(p);

  p = realloc(NULL, 0);
  /* value of p is undefined */
  if (p != NULL) free(p);

  p = realloc(NULL, 1);
  ASSERT(p != NULL);
  *p = 'X';
  ASSERT(*p == 'X');
  p = realloc(p, 17);
  ASSERT(p != NULL);
  p[0] = 'X';
  p[16] = 'Z';
  ASSERT(p[0] == 'X');
  ASSERT(p[16] == 'Z');

  /* Semantic of realloc(3) for size 0 is implementation-specific. */
  {
    void *q = realloc(p, 0);
    if (q != NULL) {
      /* What to do with p (freeing or not freeing it) is not clear:
       * specs seem to differ slightly.
       * In one interpretation, we could:
       *   ASSERT(q == p);
       *   free(p);
       * But I prefer not to assume that, considering the specs are somewhat
       * vague.
       * And in C23, this becomes undefined behavior according to
       * https://en.cppreference.com/w/c/memory/realloc
       */
    }
  }

  for (i = 4; i < 5 * 1024 * 1024; i += (64 + i / 100)) {
    const int realloc_sz = 5 * 1024 * 1024 - i;
    p = malloc(i);
    ASSERT(p != NULL);
    ASSERT(((intptr_t)p & (ALIGNMENT - 1)) == 0);
    memset(p, 0x5a, i);
    *p = 'X';
    p[i - 1] = 'Z';
    ASSERT(*p == 'X');
    ASSERT(p[1] == 0x5a);
    ASSERT(p[i - 2] == 0x5a);
    ASSERT(p[i - 1] == 'Z');

    p = realloc(p, realloc_sz);
    if (i <= realloc_sz) {
      *p = 'X';
      p[i - 1] = 'Z';
      ASSERT(*p == 'X');
      ASSERT(p[1] == 0x5a);
      ASSERT(p[i - 2] == 0x5a);
      ASSERT(p[i - 1] == 'Z');
    } else {
      ASSERT(*p == 'X');
      ASSERT(p[1] == 0x5a);
      ASSERT(p[realloc_sz - 1] == 0x5a);
    }

    free(p);
  }

  printf("Done with malloc/free tests.\n");
}

static void test_calloc() {
  char *buf = calloc(3, sizeof(int));
  ASSERT(!memcmp(buf, "\x00\x00\x00", 3));
  buf[0] = 1;
  buf[1] = 2;
  buf[2] = 3;
  ASSERT(buf[0] == 1 && buf[1] == 2 && buf[2] == 3);
#ifdef NNLC_MALLOC
  // It turns out the old calloc in crosstool fails to catch the overflow.  This
  // has been fixed in newer versions of gcc's libc.
  size_t large = ((size_t)-1) >> 5;
  size_t many = ((size_t)-1) >> 4;
  void* buf2 = calloc(many, large);
  ASSERT(buf2 == NULL);
#endif
  free(buf);
}

static int timespec_cmp(const struct timespec *a,
                        const struct timespec *b) {
  if (a->tv_sec < b->tv_sec)
    return -1;
  else if (a->tv_sec > b->tv_sec)
    return 1;
  else if (a->tv_nsec < b->tv_nsec)
    return -1;
  else if (a->tv_nsec > b->tv_nsec)
    return 1;
  return 0;
}

static void test_sleep() {
  int i;
  time_t t1, t2;
  struct timespec tp1, tp2;
  struct timeval tv1, tv2, tvdiff, tv2check;

  printf("sleep(0)...\n");
  sleep(0);
  printf("done. sleep(1)...\n");
  sleep(1);
  printf("done. sleep(3)...\n");
  sleep(3);
  printf("done. 10 * usleep(200*1000)...\n");
  ASSERT(0 == clock_gettime(CLOCK_MONOTONIC, &tp1));
  ASSERT(0 == gettimeofday(&tv1, NULL));
  t1 = time(NULL);
  for (i = 0; i < 10; ++i) usleep(200 * 1000);  /* ~ 2s */
  ASSERT(0 == clock_gettime(CLOCK_MONOTONIC, &tp2));
  ASSERT(0 == gettimeofday(&tv2, NULL));
  time(&t2);
  /* the following will work only if we are guaranteed that tp2 > tp1
   * + 1s on EFI (clock resolution is 1s). */
  printf("start time was: tp=%ld.%09lds tv=%ld.%06lds\n",
         tp1.tv_sec, tp1.tv_nsec,
         tv1.tv_sec, tv1.tv_usec);
  timersub(&tv2, &tv1, &tvdiff);
  printf("end time: %ld.%09lds tv=%ld.%06lds (tvdiff=%ld.%06lds)\n",
         tp2.tv_sec, tp2.tv_nsec,
         tv2.tv_sec, tv2.tv_usec,
         tvdiff.tv_sec, tvdiff.tv_usec);
  ASSERT(timespec_cmp(&tp1, &tp2) < 0);
  ASSERT(timercmp(&tv1, &tv2, <));
  ASSERT(t2 >= t1 + 1);
  /* check that timeradd(t1, timesub(t2, t1)) == t2 */
  timeradd(&tv1, &tvdiff, &tv2check);
  ASSERT(timercmp(&tv2check, &tv2, ==));
  printf("done.\n");
}

int main() {
  test_malloc_free();
  test_calloc();
  test_sleep();

  printf("That's all, folks!\n");
  return 0;
}
