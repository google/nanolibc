# "Nano" C library

<!--*
# Document freshness: For more information, see go/fresh-source.
freshness: { owner: 'decot' reviewed: '2021-02-25' }
*-->

go/nanolibc

## Description

This implements a small subset of a `POSIX` `C` library, mainly:

*   `printf` and `sprintf` function family
*   most common `string.h` functions: `memcpy`, `memcmp`, `strcpy`, `strncpy`,
    `strcmp`, etc.
*   `strtol`/`strtoul`/`strtoll`/`strtoull` and `atol`
*   `ctype` (no locale/encoding support)
*   a small subset of `stdio.h`: only `stdout` and `stderr` supported,
    `fwrite`/`putc`/etc. work on them
*   a tiny subset of C++ STL: `vector<>`, `auto_ptr<>`
*   a few functions are defined but their implementation is an empty shell
    (return error): `signal`, `fopen`/`fclose`/`fread`/`fseek`, `getenv`,
    `localtime`, `isatty`. See `c/unsup.c`

It should allow to compile C code that depends on a limited C POSIX subset
without any modification.

This library takes the form of a generic archive, plus a per-target runtime
adapter library. The interface of the per-target runtime library is defined as a
structure of function pointers in `c/libc.h`. For now, EFI 64 target is the main
target. We also provide a convenient tuntime library for testing, allowing to
run nanolibc-based program on linux.

## Source layout

Code lives in:

*   `c/include/`: declarations for the C library
*   `c/noimpl/`: empty header files for legacy code that includes them without
    using them
*   `c/nostd/`: google-specific header files, should work with any C library
*   `c/*.c`: implementation of the C library
*   `cxx/include/`: declarations for the C++ library
*   `cxx/noimpl/`: empty header files for legacy code that includes them without
    using them
*   `cxx/nostd/`: google-specific header files, should work with any C library
*   `cxx/*.cc`: implementation of the C++ library
*   `efi/`: code for EFI runtime adapter library, ie. implements interface
    `c/libc.h` for EFI
*   `native/`: for testing purposes only, runtime that allows to load and run a
    nanolibc-based program from linux, natively. Useful when debugging (eg. to
    use `valgrind`) or testing (eg. code coverage)

In `tests/` directory, a few unit tests are implemented that provide good
coverage of the core nanolibc functionality. The `BUILD` file in this directory
builds 3 versions of the tests:

*   "*native*": tests don't actually include/link against nanolibc, run directly
    on the host OS. Convenient to make sure unit tests work as expected against
    usual (linux/eglibc) standards
*   "*nanolib*": tests are compiled and linked against the `nanolibc` objects
    and `native/start_main.o`, ie. they are piggybacked into a normal host linux
    libc executable. This means that the tests will be able to fully exercise
    `nanolibc` code (object files are actually the same used for EFI builds as
    well) from within a normal linux host. Allows to test `nanolibc` under
    `valgrind` (TODO: add support for blaze coverage info).
*   "*efi*": tests compiled and linked against `nanolib` with a small adapter
    for `gnu-efi`, take the form of EFI module ready to be executed in `qemu` or
    on real hardware.

In `example/` directory: a simple "`hello world`" EFI & Linux program that is
built with nanlolibc.

--

decot@ 12/2016
