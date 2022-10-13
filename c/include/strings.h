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

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_STRINGS_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_STRINGS_H_

#include <sys/_cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

int strcasecmp(const char *p1, const char *p2);
int strncasecmp(const char *s1, const char *s2, size_t n);

__END_DECLS

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_STRINGS_H_
