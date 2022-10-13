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
 * Implementation of the EFI runtime wrappers for nanolibC.
 */

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <efi.h>
#include <efilib.h>

#include "third_party/nanolibc/efi/gnu_efi_crt.h"

#if defined(EFI_DEBUG) && (EFI_DEBUG >= 9)
#define EFI_DEBUG_WAIT_GDB
#endif

/* Externs */
extern int main(int argc, char *argv[]);

/* Global state for EFI wrappers */
static struct {
  struct nnlc_sysdeps nanolibc_sysdeps;
  int main_retval;
} private_nnlc_efi_context;

struct nnlc_efi_context __nnlc_efi_context;

/*
 * Input/Output
 */

/* Any C-string printed by the nanolibc is split into chunks, and each
 * chunk is converted to UCS-2 before being sent to the EFI
 * output/error streams. The size of chunk is: */
#define _EFI_IO_CHUNK8_SIZE                 \
  64 /* any size >= 1 Ok. Best result if >= \
      * vfprintf's internal buffer (see stdio.c) */

/* Internal function to convert C-string s8 of size 'len' bytes into
 * UCS-2 string s16, including trailing \0. return number of chars
 * converted (excluding trailing \0), not the number of wchars
 * actually stored.
 * s16 must be at least 2*len + 1 wchars for \n -> \r\n conversion and
 * trailing \0 */
static size_t conio_a2u(CHAR16 *s16, const char *s8, size_t len) {
  size_t i;

  /* IMPORTANT: By contract, s16 can hold at least 2*len + 1 wchars
   * (ie. 4*len + 2 bytes). */

  for (i = 0; (i < len) && *s8; ++i) {
    if (*s8 == '\n') *s16++ = (CHAR16)'\r';
    *s16++ = (CHAR16)*s8++;
  }

  *s16 = (CHAR16)'\0';
  return i;
}

/* Abstraction for a EFI output/error stream */
typedef void (*conio_writer_t)(CHAR16 const *s);

/* Convert a C-string buffer s8 of sz bytes into CHAR16 chunks of size
 * _EFI_IO_CHUNK8_SIZE and write each of them */
static void efi_conio_write(conio_writer_t writer, const char *s8, size_t sz) {
  CHAR16 s16[_EFI_IO_CHUNK8_SIZE * 2 /* for \n -> \r\n */ + 1 /* \0 */];

  while (sz > 0) {
    size_t converted = conio_a2u(
        s16, s8, (_EFI_IO_CHUNK8_SIZE > sz) ? sz : _EFI_IO_CHUNK8_SIZE);
    if (converted <= 0) break;

    writer(s16);

    /* This was the last chunk? */
    if (converted >= sz) break;

    s8 += converted;
    sz -= converted;
  }
}

/* conio_writer_t for console standard output */
static void efi_stdout_writer(CHAR16 const *s) { Output((CHAR16 *)s); }

/* conio_writer_t for console standard error */
static void efi_stderr_writer(CHAR16 const *s) {
  SIMPLE_TEXT_OUTPUT_INTERFACE *conerr = __nnlc_efi_context.efi_systab->StdErr;
  uefi_call_wrapper(conerr->OutputString, 2, conerr, (CHAR16 *)s);
}

/* nanolibc wrapper to print C-strings onto EFI standard output stream */
static ssize_t write_stdout(const void *s8, size_t sz) {
  efi_conio_write(efi_stdout_writer, s8, sz);
  return sz;
}

/* nanolibc wrapper to print C-strings onto EFI standard error stream */
static ssize_t write_stderr(const void *s8, size_t sz) {
  efi_conio_write(efi_stderr_writer, s8, sz);
  return sz;
}

/*
 * underlying implementation for exit() wrapper.
 *
 * efi_exit() below returns to the parent frame owning the
 * EFI_NANOLIBC_TRY/EFI_NANOLIBC_CATCH blocks, which are in main() by
 * default.
 *
 * Background: We don't directly call BS->Exit() here because we want
 * to allow DXE driver routines that call into a nanlolibc code to
 * return cleanly from a protocol method when exit() is called,
 * instead of unloading the driver, causing an exception or
 * terminating the calling program.
 */
static void efi_exit(int status) {
  private_nnlc_efi_context.main_retval = status;
  EFI_NANOLIBC_THROW();
}

/*
 * usleep()
 */

static uint64_t efi_usleep64(uint64_t micro_seconds) {
  uefi_call_wrapper(BS->Stall, 1, micro_seconds);
  return 0;
}

/*
 * gettime_wall()
 *
 * Credits: Linux kernel for mktime(), Gauss (?).
 */
static int efi_gettime_wall(uint64_t *secs, uint64_t *nanosecs) {
  EFI_TIME t;
  EFI_STATUS Status = uefi_call_wrapper(RT->GetTime, 2, &t, NULL);
  if (EFI_ERROR(Status))
    return -1;

  /* 1..12 -> 11,12,1..10 */
  if (0 >= (int) (t.Month -= 2)) {
    t.Month += 12;      /* Puts Feb last since it has leap day */
    t.Year -= 1;
  }

  *secs = ((((uint64_t)
             (t.Year/4 - t.Year/100 + t.Year/400 + 367*t.Month/12 + t.Day) +
             t.Year*365 - 719499
             )*24 + t.Hour
            )*60 + t.Minute
           )*60 + t.Second;

  if (t.TimeZone != EFI_UNSPECIFIED_TIMEZONE)
    *secs += t.TimeZone*60;

  *nanosecs = t.Nanosecond;
  return 0;
}

/*
 * efi_main(): initialize nanolibc() and call C main() function.
 */

/*
 * Optional function called to pause efi_main() right after it
 * started, before nanolibc is initialized. Use it with a target
 * remote gdb to set breakpoints and continue execution: see
 * instructions below.
 */
#ifdef EFI_DEBUG_WAIT_GDB
/* http://wiki.osdev.org/UEFI:
     qemu
     # "Image base: 0x2E8F4000"
     gdb target/gnu-efi/t0.efi
     info file
     # 0x0000000000003000 - 0x000000000000c993 is .text
     # ...
     # 0x000000000000e000 - 0x000000000000fd60 is .data
     file
     add-symbol-file targets/gnu-efi/t0.efi.debug \
       (0x2E8F4000+0x0000000000003000) \
       -s .data (0x2E8F4000+0x000000000000e000)
     set architecture i386:x86-64:intel
     target remote :1234
     display/i $pc
     set variable efi_debug_wait=0
     // or, out of context: set $eax=0 before 'test...'
     b .....
     c/si/s...
 */
extern const char ImageBase[]; /* elf_x86_64_efi.lds */
static void efi_start_debug() {
  Print(L"Image base: 0x%llx\n", &ImageBase);
  Print(L"Waiting for gdb to set variable wait = 0...\n");

  static volatile int efi_debug_wait = 1;
  while (efi_debug_wait) {
    __asm__ __volatile__("pause" :: : "memory");
  }

  Print(L"Continuing...\n");
}
#endif

/* convert EFI argc/argv into C argc/argv: we assume that all UCS-2
 * chars in EFI argc are simple ascii. Memory is dynamically allocated
 * for pointer argv, and not freed. */
static int retrieve_argv(char **argv[]) {
  INTN Argc;
  CHAR16 **Argv;
  INTN i;
  size_t sz = 0;
  void *data;
  char *buff;

  *argv = NULL;

  Argc = GetShellArgcArgv(__nnlc_efi_context.efi_image, &Argv);
  if (Argc < 0) return -1;

  /* Determine size needed to store argv */
  for (i = 0; i < Argc; ++i) sz += StrLen(Argv[i]) + 1 /* trailing \0 */;

  /* We expect at least argv[0] */
  if (sz <= 0) return -1;

  /* Allocate argv[] + argv strings + a trailing NULL ptr */
  data = malloc(((Argc + 1) * sizeof(char *)) + sz);
  if (!data) return -1;

  *argv = (char **)data;
  buff = (char *)(data + (Argc + 1) * sizeof(char *));

  /* Prepare argv[] and "convert" wchar to char: we assume all wchars
   * contain only ASCII. */
  for (i = 0; i < Argc; ++i) {
    CHAR16 *s16 = Argv[i];
    (*argv)[i] = buff;
    while (*s16) *buff++ = *s16++;
    *buff++ = '\0';
  }

  /* append a trailing NULL ptr (is at Argc index) */
  (*argv)[i] = NULL;

  return Argc;
}

/* Actually call the main() function, catching calls to exit in an
 * EFI_NANOLIBC_TRY/EFI_NANOLIBC_CATCH construct (see comments for
 * efi_exit() above) */
static int call_main(int argc, char **argv) {
  private_nnlc_efi_context.main_retval = EXIT_FAILURE;

  EFI_NANOLIBC_TRY
  { private_nnlc_efi_context.main_retval = main(argc, argv); }
  EFI_NANOLIBC_CATCH
  {
    /* catch any THROW, exit() is one of the throwers, expected to set
     * main_retval properly */
    fprintf(stderr, "exit(%d) was called\n",
            private_nnlc_efi_context.main_retval);
  }
  EFI_NANOLIBC_ETRY

  return private_nnlc_efi_context.main_retval;
}

/*
 * Top-level wrapper around main() that initializes the libC and calls
 * main(). Usually used as efi_main().
 */
EFI_STATUS
libnanolibc_start(EFI_HANDLE image, EFI_SYSTEM_TABLE *systab) {
  int argc;
  char **argv = NULL;
  int rc;

  InitializeLib(image, systab);

#ifdef EFI_DEBUG_WAIT_GDB
  efi_start_debug();
#endif

  __nnlc_efi_context.efi_image = image;
  __nnlc_efi_context.efi_systab = systab;
  private_nnlc_efi_context.main_retval = EXIT_FAILURE;
  private_nnlc_efi_context.nanolibc_sysdeps.malloc = AllocatePool;
  private_nnlc_efi_context.nanolibc_sysdeps.free = FreePool;

  /* ConOut may be NULL (eg. b/22847275). In that case,
   * refuse to go any further */
  if (NULL == __nnlc_efi_context.efi_systab->ConOut)
      return EFI_OUT_OF_RESOURCES;
  private_nnlc_efi_context.nanolibc_sysdeps.write_stdout = write_stdout;

  /* StdErr can be NULL (eg. ikaria): redirect to stdout in that case */
  if (__nnlc_efi_context.efi_systab->StdErr)
    private_nnlc_efi_context.nanolibc_sysdeps.write_stderr = write_stderr;
  else
    private_nnlc_efi_context.nanolibc_sysdeps.write_stderr = write_stdout;

  private_nnlc_efi_context.nanolibc_sysdeps.exit = efi_exit;
  private_nnlc_efi_context.nanolibc_sysdeps.usleep = efi_usleep64;
  private_nnlc_efi_context.nanolibc_sysdeps.gettime_monotonic
      = efi_gettime_wall;  /* nothing better from UEFI */
  private_nnlc_efi_context.nanolibc_sysdeps.gettime_wall
      = efi_gettime_wall;

  rc = _nnlc_initialize(&private_nnlc_efi_context.nanolibc_sysdeps);
  if (rc) return EFI_LOAD_ERROR;

  argc = retrieve_argv(&argv);
  if (argc < 0) fprintf(stderr, "Warning: cannot retrieve argc/argv\n");

  rc = call_main(argc, argv);

  /* Careful with what we return from efi_main: only
   * success/unsupported allowed! */
  if (rc != EXIT_SUCCESS) {
    fprintf(stderr, "main() failed with return value: %d\n", rc);
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

#ifdef COVERAGE
/*
 * Hooks needed when compiled with gcov support.
 */
void __gcov_init(void *x) { (void)x; }

void __gcov_merge_add(void *x, unsigned u) {
  (void)x;
  (void)u;
}
#endif
