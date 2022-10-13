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
 * Definition of the EFI runtime global wrappers and data for nanolibC.
 */

#ifndef THIRD_PARTY_NANOLIBC_EFI_GNU_EFI_CRT_H_
#define THIRD_PARTY_NANOLIBC_EFI_GNU_EFI_CRT_H_

#include <stdint.h>

#include "third_party/nanolibc/c/libc.h"

__BEGIN_DECLS

/* Required here (in extern "C" + after stdint.h) because of weird EFI
 * practices in header files */
#include <efi.h>
#include <efisetjmp.h>

struct nnlc_efi_context {
  EFI_HANDLE efi_image;
  EFI_SYSTEM_TABLE* efi_systab;

  /* sjlj buffer used to come back to up-frame
   * EFI_NANOLIBC_TRY/EFI_NANOLIBC_CATCH block when
   * EFI_NANOLIBC_THROW() is called. Used mainly to be able to catch
   * calls to exit(), which does this EFI_NANOLIBC_THROW() */
  jmp_buf exc_jmp_buf;
};
extern struct nnlc_efi_context __nnlc_efi_context;

/* Poor's man Exception-like mechanism, mainly to catch calls to
 * exit(). The "catch" statement is not required but recommended for
 * idiomatic reasons. CAN NOT BE NESTED. */
#define EFI_NANOLIBC_TRY    if (0 == setjmp(&__nnlc_efi_context.exc_jmp_buf)) {
#define EFI_NANOLIBC_CATCH  } else {
#define EFI_NANOLIBC_ETRY   }

#define EFI_NANOLIBC_THROW() longjmp(&__nnlc_efi_context.exc_jmp_buf, 1)

/* Helper macro on top of try/catch to call given function, returns
 * given retval_failure in case of return value != 0 or early exit() */
#define EFI_NANOLIBC_CALL(retval_failure, fct, args...)     \
  ({                                                        \
    EFI_STATUS block_retval;                                \
    EFI_NANOLIBC_TRY {                                      \
      if (0 == (fct)(args))                                 \
        block_retval = EFI_SUCCESS;                         \
      else                                                  \
        block_retval = (retval_failure);                    \
    } EFI_NANOLIBC_CATCH {                                  \
      block_retval = (retval_failure);                      \
    } EFI_NANOLIBC_ETRY                                     \
    block_retval;                                           \
  })

__END_DECLS

#endif  // THIRD_PARTY_NANOLIBC_EFI_GNU_EFI_CRT_H_
