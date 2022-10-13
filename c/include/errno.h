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

#ifndef THIRD_PARTY_NANOLIBC_C_INCLUDE_ERRNO_H_
#define THIRD_PARTY_NANOLIBC_C_INCLUDE_ERRNO_H_

#include <sys/_cdefs.h>

#define EPERM 1
#define ENOENT 2
#define EINTR 4
#define EIO 5
#define ENXIO 6
#define EBADF 9
#define EAGAIN 11
#define ENOMEM 12
#define EFAULT 14
#define EBUSY 16
#define ENODEV 19
#define EINVAL 22
#define ERANGE 34

#define ENOSYS 38
#define EOPNOTSUPP 95
#define ETIMEDOUT 110

__BEGIN_DECLS

extern int errno; /* non MT-safe */

__END_DECLS

#endif  // THIRD_PARTY_NANOLIBC_C_INCLUDE_ERRNO_H_
