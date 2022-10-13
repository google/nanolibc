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

#include <ctype.h>
#include <endian.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <efi.h>
#include <efiapi.h>
#include <efidef.h>
#include <efilib.h>

#include "platforms/bios/standalone_efi/shared/include/google_call_wrappers.h"
#include "third_party/nanolibc/efi/ggl_pci.h"
#include "third_party/nanolibc/efi/gnu_efi_crt.h"

struct ggl_pci_locator {
  UINTN domain;
  UINTN bus;
  UINTN device;
  UINTN function;
};

struct ggl_pci_device {
  struct ggl_pci_locator location;
  EFI_HANDLE Handle;
  EFI_PCI_IO *PciIo;
};

/* Accepts 0000:00:00.0 and 00:00.0 formats, returns 0 for success */
static int parse_pci_address(const char *device_locator,
                             struct ggl_pci_locator *result) {
  UINTN items[4];
  int nitems = 3;  // by default, expecting format "00:00.0" (3 components)
  char *endptr;

  // Expecting either domain or bus, followed by a ':'
  if (!device_locator || !isxdigit(*device_locator)) {
    return -1;  // expected at least one digit
  }
  items[0] = strtoul(device_locator, &endptr, 16);
  if (!endptr || (*endptr != ':')) {
    return -1;
  }

  // expecting either bus or device,
  // followed by a '.' (--> nitems == 3) or a ':' (--> nitems == 4)
  if (!isxdigit(endptr[1])) {
    return -1;  // expected at least one digit
  }
  items[1] = strtoul(endptr + 1, &endptr, 16);
  if (!endptr || !*endptr) {
    return -1;
  } else if (*endptr == ':') {
    nitems = 4;  // a ':' follows, so we were given format "0000:00:00.0"
  } else if (*endptr != '.') {
    return -1;  // else we were expecting the '.' of "00:00.0" format: fail
  }

  // expecting either device (nitems == 4) or function (nitems == 3)
  if (!isxdigit(endptr[1])) {
    return -1;  // expected at least one digit
  }
  items[2] = strtoul(endptr + 1, &endptr, 16);
  if (!endptr) {
    return -1;
  }

  if (nitems == 3) {
    // not expecting anything else after the function
    if (*endptr != '\0') {
      return -1;
    }
  } else {
    // now expecting a '.' and the function after it:
    if ((*endptr != '.') || !isxdigit(endptr[1])) {
      return -1;  // expected at least '.' and one digit
    }
    items[3] = strtoul(endptr + 1, &endptr, 16);
    if (!endptr || *endptr != '\0') {
      return -1;  // expected end of string
    }
  }

  if (nitems == 3) {
    result->domain = 0;
    result->bus = items[0];
    result->device = items[1];
    result->function = items[2];
  } else {
    result->domain = items[0];
    result->bus = items[1];
    result->device = items[2];
    result->function = items[3];
  }

  // check validity
  if (result->domain & ~0xffffU) return -1;
  if (result->bus & ~0xffU) return -1;
  if (result->device & ~0xffU) return -1;
  if (result->function & ~0x7U) return -1;
  return 0;
}

/* Return 0 if specified location at pdev->location contains a PCI device,
 * update pdev in that case. */
static int lookup_pci_device(struct ggl_pci_device *pdev) {
  EFI_STATUS Status;
  UINTN i, found;
  UINTN NumHandles = 0;
  EFI_HANDLE *HandleBuffer;

  fprintf(stderr,
          "Preparing to access PCI device at %04lx:%02lx:%02lx.%lx...\n",
          pdev->location.domain, pdev->location.bus, pdev->location.device,
          pdev->location.function);

  /* Get a list of all devices that support the PCI IO Protocol */
  Status = GOOGLE_UEFI_CALL(BS->LocateHandleBuffer, ByProtocol, &PciIoProtocol,
                            NULL, &NumHandles, &HandleBuffer);

  if (EFI_ERROR(Status)) {
    fprintf(stderr, "Error locating pci io compatible handles, Status: %d\n",
            (int)Status);
    return -1;
  }

  for (found = FALSE, i = 0; !found && (i < NumHandles); ++i) {
    EFI_PCI_IO *PciIo = NULL;
    UINTN IdSeg = 0;
    UINTN IdBus = 0;
    UINTN IdDev = 0;
    UINTN IdFunc = 0;

    /* Open the PCI IO protocol */
    Status = GOOGLE_UEFI_CALL(BS->OpenProtocol, HandleBuffer[i], &PciIoProtocol,
                              (void **)&PciIo, __nnlc_efi_context.efi_image,
                              NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (EFI_ERROR(Status)) {
      fprintf(stderr, "Error opening device %u: %d\n", (unsigned)i,
              (int)Status);
      continue;
    }

    Status = GOOGLE_UEFI_CALL(PciIo->GetLocation, PciIo, &IdSeg, &IdBus, &IdDev,
                              &IdFunc);
    if (EFI_ERROR(Status)) {
      fprintf(stderr, "Error reading device location for %u, Status: %d\n",
              (unsigned)i, (int)Status);
    } else {
      /* Found it? */
      if ((IdSeg == pdev->location.domain) && (IdBus == pdev->location.bus) &&
          (IdDev == pdev->location.device) &&
          (IdFunc == pdev->location.function)) {
        pdev->Handle = HandleBuffer[i];
        pdev->PciIo = PciIo;
        found = TRUE;
        break;
      }
    }

    GOOGLE_UEFI_CALL(BS->CloseProtocol, HandleBuffer[i], &PciIoProtocol,
                     __nnlc_efi_context.efi_image, NULL);
  }

  GOOGLE_UEFI_CALL(BS->FreePool, HandleBuffer);
  return (found ? 0 : -1);
}

static int enable_pci_accesses(struct ggl_pci_device *pdev) {
  EFI_STATUS Status;
  UINT64 AttrValues = 0;

  Status = GOOGLE_UEFI_CALL(
      pdev->PciIo->Attributes, pdev->PciIo, EfiPciIoAttributeOperationEnable,
      EFI_PCI_IO_ATTRIBUTE_MEMORY | EFI_PCI_IO_ATTRIBUTE_BUS_MASTER,
      &AttrValues);
  if (EFI_ERROR(Status)) {
    fprintf(stderr, "Could not set PCI device attrs (res=0x%llx): %d\n",
            (unsigned long long int)AttrValues, (int)Status);
    return -1;
  }

  return 0;
}

struct ggl_pci_device *ggl_pci_open(const char *device_locator) {
  struct ggl_pci_device *pdev;

  pdev = malloc(sizeof(*pdev));
  if (!pdev) {
    return NULL;
  }

  if (parse_pci_address(device_locator, &pdev->location)) {
    fprintf(stderr, "Error parsing PCI address '%s'\n", device_locator);
    free(pdev);
    return NULL;
  }

  if (lookup_pci_device(pdev)) {
    fprintf(stderr, "Cannot open PCI device\n");
    free(pdev);
    return NULL;
  }

  if (enable_pci_accesses(pdev)) {
    fprintf(stderr, "Error enabling PCI device\n");
    ggl_pci_close(pdev);
    return NULL;
  }

  return pdev;
}

int ggl_pci_close(struct ggl_pci_device *pdev) {
  if (pdev->PciIo) {
    GOOGLE_UEFI_CALL(BS->CloseProtocol, pdev->Handle, &PciIoProtocol,
                     __nnlc_efi_context.efi_image, NULL);
  }
  free(pdev);
  return 0;
}

void *ggl_pci_efi_pciio(struct ggl_pci_device *pdev) { return pdev->PciIo; }

int ggl_pci_cfgread16(struct ggl_pci_device *pdev, unsigned offset,
                      uint16_t *val) {
  EFI_STATUS Status;
  UINT16 Data = 0xa5a5;

  Status = GOOGLE_UEFI_CALL(pdev->PciIo->Pci.Read, pdev->PciIo,
                            EfiPciIoWidthUint16, offset, 1, &Data);
  if (EFI_ERROR(Status)) {
    return -1;
  }

  *val = le16toh(Data);
  return 0;
}

int ggl_pci_cfgwrite16(struct ggl_pci_device *pdev, unsigned offset,
                       uint16_t val) {
  EFI_STATUS Status;
  UINT16 Data = htole16(val);

  Status = GOOGLE_UEFI_CALL(pdev->PciIo->Pci.Write, pdev->PciIo,
                            EfiPciIoWidthUint16, offset, 1, &Data);
  if (EFI_ERROR(Status)) {
    return -1;
  }

  return 0;
}

int ggl_pci_cfgread32(struct ggl_pci_device *pdev, unsigned offset,
                      uint32_t *val) {
  EFI_STATUS Status;
  UINT32 Data = 0xa5a5a5a5;

  Status = GOOGLE_UEFI_CALL(pdev->PciIo->Pci.Read, pdev->PciIo,
                            EfiPciIoWidthUint32, offset, 1, &Data);
  if (EFI_ERROR(Status)) {
    return -1;
  }

  *val = le32toh(Data);
  return 0;
}

int ggl_pci_cfgwrite32(struct ggl_pci_device *pdev, unsigned offset,
                       uint32_t val) {
  EFI_STATUS Status;
  UINT32 Data = htole32(val);

  Status = GOOGLE_UEFI_CALL(pdev->PciIo->Pci.Write, pdev->PciIo,
                            EfiPciIoWidthUint32, offset, 1, &Data);
  if (EFI_ERROR(Status)) {
    return -1;
  }

  return 0;
}

int ggl_pci_mread32(struct ggl_pci_device *pdev, unsigned bar_index,
                    uint64_t offset, uint32_t *val) {
  EFI_STATUS Status;
  UINT32 Data = 0xa5a5a5a5;

  Status = GOOGLE_UEFI_CALL(pdev->PciIo->Mem.Read, pdev->PciIo,
                            EfiPciIoWidthUint32, bar_index, offset, 1, &Data);
  if (EFI_ERROR(Status)) {
    return -1;
  }

  *val = Data;
  return 0;
}

int ggl_pci_mwrite32(struct ggl_pci_device *pdev, unsigned bar_index,
                     uint64_t offset, uint32_t val) {
  EFI_STATUS Status;
  UINT32 Data = val;

  Status = GOOGLE_UEFI_CALL(pdev->PciIo->Mem.Write, pdev->PciIo,
                            EfiPciIoWidthUint32, bar_index, offset, 1, &Data);
  if (EFI_ERROR(Status)) {
    return -1;
  }

  return 0;
}

int ggl_pci_mpoll32(struct ggl_pci_device *pdev, unsigned bar_index,
                    unsigned bar_offset, uint32_t mask, uint32_t expected_val,
                    unsigned timeout_usecs, uint32_t *last_val) {
  EFI_STATUS Status;
  UINT64 Data = expected_val & mask;
  UINT64 Result = ~0;

  Status = GOOGLE_UEFI_CALL(pdev->PciIo->PollMem, pdev->PciIo,
                            EfiPciIoWidthUint32, bar_index, bar_offset, mask,
                            Data, timeout_usecs * 10, &Result);
  if (last_val) *last_val = Result;

  if (EFI_TIMEOUT == Status) {
    return 1;
  } else if (EFI_ERROR(Status)) {
    return -1;
  }

  return 0;
}
