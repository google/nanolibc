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

#ifndef THIRD_PARTY_NANOLIBC_EFI_GGL_PCI_H_
#define THIRD_PARTY_NANOLIBC_EFI_GGL_PCI_H_

#include <stddef.h>
#include <stdint.h>

__BEGIN_DECLS

//
// Platform-agnostic functions
//

// IMPORTANT Foreword: a few words about EFI "bar index" vs "BARx. In the
// normal world, the 'x' in "BARx" is used to denote an offset in BAR space:
// the x-th dword. Under Linux, this translates into the
// /sys/bus/pci/devices/BBDDf/resourceX attribute.
// Because resource registers can be 32-bit or 64-bit wide, the BAR at offset X
// doesn't mean that it's the X'th BAR of the device. Now, for the UEFI crew,
// this BAR ordinal "index" is precisely what they are manipulating.
//
// A few examples:
//
// gq:
//  EFI BAR Index 0: BAR Offset 0: Memory 64-bit
//  EFI BAR Index 1: BAR Offset 2: Memory 32-bit
//  EFI BAR Index 2: BAR Offset 3: EMPTY Memory 32-bit (not available)
//  EFI BAR Index 3: BAR Offset 4: Memory 64-bit
//
// swinetrek:
//  EFI BAR Index 0: BAR Offset 0: Memory 64-bit
//  EFI BAR Index 1: BAR Offset 2: Memory 64-bit
//  EFI BAR Index 2: BAR Offset 4: Memory 64-bit

struct ggl_pci_device;

// Create a ggl_pci_device instance to interface with PCI device whose
// location is given by "device_locator". Returns NULL if device not available.
// Supported formats for "device_locator":
//  - DDDD:BB:dd.f
//  - BB:dd.f
struct ggl_pci_device *ggl_pci_open(const char *device_locator);

int ggl_pci_close(struct ggl_pci_device *pdev);

// read 16-bit config register value located at given byte offset
// returns 0 upon success, anything else for failure
// Note 1: *val updated in host byte order (internally converted from LE)
// Note 2: *val not modified in case of failure
int ggl_pci_cfgread16(struct ggl_pci_device *pdev, unsigned offset,
                      uint16_t *val);

// write 16-bit config register value located at given byte offset
// returns 0 upon success, anything else for failure
// Note: val expected to be in host byte order (internally converted to LE)
int ggl_pci_cfgwrite16(struct ggl_pci_device *pdev, unsigned offset,
                       uint16_t val);

// read 32-bit config register value located at given byte offset
// returns 0 upon success, anything else for failure
// Note 1: *val updated in host byte order (internally converted from LE)
// Note 2: *val not modified in case of failure
int ggl_pci_cfgread32(struct ggl_pci_device *pdev, unsigned offset,
                      uint32_t *val);

// write 32-bit config register value located at given byte offset
// returns 0 upon success, anything else for failure
// Note: val expected to be in host byte order (internally converted to LE)
int ggl_pci_cfgwrite32(struct ggl_pci_device *pdev, unsigned offset,
                       uint32_t val);

// read 32-bit mmio register value located at given byte offset inside BAR
// returns 0 upon success, anything else for failure
// Note 1: BAR contents considered to be in host byte order. Use *e32_to_cpu as
//         needed if BARs use a different endianness.
// Note 2: *val not modified in case of failure
// Note 3: linux resourceX in sysfs != bar index (linux resource X is the offset
//         of the bar_index-nth BAR register)
int ggl_pci_mread32(struct ggl_pci_device *pdev, unsigned bar_index,
                    uint64_t offset, uint32_t *val);

// write 32-bit mmio register value located at given byte offset inside BAR
// returns 0 upon success, anything else for failure
// Note 1: BAR contents considered to be in host byte order. Use cpu_to_*e32 as
//         needed if BARs use a different endianness.
// Note 2: linux resourceX in sysfs != bar index (linux resource X is the offset
//         of the bar_index-nth BAR register)
int ggl_pci_mwrite32(struct ggl_pci_device *pdev, unsigned bar_index,
                     uint64_t offset, uint32_t val);

// read given mmio 32-bit register until its value & mask == expected_val & mask
// or until timeout (usecs). when last_val is non NULL, last successfully
// unmasked read value stored there.
// returns 0 if expected masked value is read before timeout, 1 upon timeout,
// anything else for error (eg. read error) in which case *last_val
// is undefined (may be modified).
// Note 1: BAR contents considered to be in host byte order. Use cpu_to_*e32 and
//         *e32_to_cpu as needed if BARs use a different endianness.
// Note 2: linux resourceX in sysfs != bar index (linux resource X is the offset
//         of the bar_index-nth BAR register)
int ggl_pci_mpoll32(struct ggl_pci_device *pdev, unsigned bar_index,
                    unsigned bar_offset, uint32_t mask, uint32_t expected_val,
                    unsigned timeout_usecs, uint32_t *last_val);

// On EFI only: return underlying EFI_PCI_IO_PROTOCOL instance.
void *ggl_pci_efi_pciio(struct ggl_pci_device *pdev);

__END_DECLS

#endif  // THIRD_PARTY_NANOLIBC_EFI_GGL_PCI_H_
