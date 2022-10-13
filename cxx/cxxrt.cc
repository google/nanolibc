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
 * Implementation of C++ runtime functions.
 */

#include <assert.h>
#include <stdlib.h>

extern "C" {
void __cxa_pure_virtual(void) { assert(!"pure virtual method called\n"); }

void __cxa_deleted_virtual(void) { assert(!"deleted virtual method called\n"); }
};  // extern "C"

namespace std {
void abort() throw() __attribute__((__noreturn__));
void abort() throw() {
  assert(!"std::abort called\n");
  ::abort();
}

void terminate() throw() __attribute__((__noreturn__));
void terminate() throw() { std::abort(); }

}  // namespace std

/* operator new() borrowed from eglibc */
void *operator new(size_t sz) {
  void *p;

  /* malloc (0) is unpredictable; avoid it.  */
  if (sz == 0) sz = 1;
  p = reinterpret_cast<void *>(::malloc(sz));
  assert(p != NULL);
  return p;
}

void *operator new [](size_t sz) { return ::operator new(sz); }

void
operator delete(void *ptr) throw() {
  if (ptr) ::free(ptr);
}

void operator delete[](void *ptr) throw() { ::operator delete(ptr); }
