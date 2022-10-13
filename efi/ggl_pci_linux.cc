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

// A userspace implementation of the EFI PCI interface for testing purposes.
#include <endian.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

#include "file/base/file_closer.h"
#include "file/base/filelineiter.h"
#include "file/base/path.h"
#include "file/util/linux_fileops.h"
#include "strings/numbers.h"
#include "third_party/absl/status/status.h"
#include "third_party/absl/strings/numbers.h"
#include "third_party/absl/strings/str_cat.h"
#include "third_party/absl/strings/str_split.h"
#include "third_party/absl/strings/string_view.h"
#include "third_party/absl/time/clock.h"
#include "third_party/absl/time/time.h"
#include "third_party/nanolibc/efi/ggl_pci.h"
#include "util/endian/endian.h"
#include "util/gtl/ptr_util.h"

using ::file_util::LinuxFileOps;

namespace {
const int kMaxPciBars = 6;

// Accepts 0000:00:00.0 and 00:00.0 formats, returns the sysfs path to the PCI
// device for success and empty string on failure.
std::string parse_pci_address(std::string device_locator) {
  static const char path_prefix[] = "/sys/bus/pci/devices/";
  const int locator_parts =
      std::count(device_locator.begin(), device_locator.end(), ':') + 1;

  // Device locator must have either two or three parts, separated by ":".
  if (locator_parts != 2 && locator_parts != 3) {
    return "";
  }

  if (locator_parts == 2) {
    device_locator = absl::StrCat("0000:", device_locator);
  }

  return absl::StrCat(path_prefix, device_locator);
}

bool FileExists(std::string filepath) {
  return access(filepath.c_str(), F_OK) == 0;
}

bool GetFdSize(int fd, size_t *size) {
  struct stat st;
  if (fstat(fd, &st) < 0) {
    PLOG(ERROR) << "Could not get config space size";
    return false;
  }
  *size = st.st_size;
  return true;
}

bool GetBARFlags(absl::string_view sysfs_dirpath,
                 uint64_t *sysfs_resource_flags /* array[kMaxPciBars] */) {
  const std::string path = file::JoinPath(sysfs_dirpath, "resource");
  File *f = nullptr;
  absl::Status open_status = file::Open(path, "r", &f, file::Defaults());
  if (!open_status.ok()) {
    LOG(ERROR) << "Could not open resource file " << path;
    return false;
  }
  // file will be closed by FileLines below

  memset(sysfs_resource_flags, 0, sizeof(*sysfs_resource_flags) * kMaxPciBars);
  int bar_offset = 0;
  for (std::string &line : FileLines(f, FileLineIterator::REMOVE_LINEFEED)) {
    std::vector<absl::string_view> v =
        absl::StrSplit(line, ' ', absl::SkipWhitespace());
    if (v.size() != 3) {
      LOG(ERROR) << "Invalid resource file format";
      return false;
    }
    if (bar_offset >= kMaxPciBars) {
      // Ignore option ROM, etc.
      break;
    }
    if (!absl::SimpleHexAtoi(v[2], &sysfs_resource_flags[bar_offset++])) {
      LOG(ERROR) << "Invalid flags in resource file";
      return false;
    }
  }

  return true;
}

inline bool Is64BitBAR(uint64_t sysfs_resource_flags) {
  return (sysfs_resource_flags & 0x7) == 0x4;
}
}  // namespace

struct ggl_pci_device {
  std::string device_path;
  int pci_config;
  size_t config_size_bytes;
  uint8_t *bars[kMaxPciBars];
  size_t bar_sizes_bytes[kMaxPciBars];
};

struct ggl_pci_device *ggl_pci_open(const char *device_locator) {
  auto pdev = ::gtl::MakeUnique<ggl_pci_device>();
  bzero(pdev.get(), sizeof(ggl_pci_device));  // Zero the struct.

  if (!pdev) {
    return nullptr;
  }

  pdev->device_path = parse_pci_address(device_locator);
  if (pdev->device_path.empty()) {
    LOG(ERROR) << "Could not parse PCI address " << device_locator;
    return nullptr;
  }

  // Get File Descriptor for PCI config space.
  const std::string config_path = file::JoinPath(pdev->device_path, "config");
  pdev->pci_config = LinuxFileOps::OpenFD(config_path, O_RDWR, 0);
  if (pdev->pci_config < 0) {
    PLOG(ERROR) << "Could not open FD for PCI Config space " << config_path;
    return nullptr;
  }
  if (!GetFdSize(pdev->pci_config, &pdev->config_size_bytes)) {
    return nullptr;
  }
  LOG(INFO) << pdev->device_path
            << " config space size: " << pdev->config_size_bytes;

  // Get basic BAR flags, in order to determine BAR index
  // Ref:
  // https://github.com/tianocore/edk2/blob/master/DuetPkg/PciBusNoEnumerationDxe/PciEnumeratorSupport.c#L858
  uint64_t sysfs_resource_flags[kMaxPciBars];
  if (!GetBARFlags(pdev->device_path, sysfs_resource_flags)) {
    return nullptr;
  }

  // Map the PCI resources in memory.
  // The tricky part is to convert Linux BAR Offset into EFI BAR Index, for
  // which we use sysfs_resource_flags[].
  const bool is_writable = true;
  const bool is_shared = true;
  for (int bar_offset = 0, bar_index = 0; bar_offset < kMaxPciBars;
       bar_offset += Is64BitBAR(sysfs_resource_flags[bar_offset]) ? 2 : 1,
       ++bar_index) {
    const std::string resource_x_path =
        absl::StrCat(pdev->device_path, "/resource", bar_offset);
    if (!FileExists(resource_x_path)) {
      // keep pdev->bars[bar_index] == NULL (from bzero above)
      continue;
    }

    VLOG(1) << device_locator << ": BAR Index for BAR" << bar_offset << ": "
            << bar_index;

    pdev->bars[bar_index] = static_cast<uint8_t *>(LinuxFileOps::MMapFile(
        resource_x_path, 0, &pdev->bar_sizes_bytes[bar_index], is_writable,
        is_shared));
    // Check if MMap failed, warn but go on if so (ggl_pci_m* will fail for
    // those BARs we failed to mmap)
    if (pdev->bars[bar_index] == nullptr) {
      LOG(WARNING) << "Failed to MMap BAR" << bar_offset
                   << "'s resource file: " << resource_x_path;
    }
  }

  return pdev.release();
}

int ggl_pci_close(struct ggl_pci_device *pdev) {
  if (!LinuxFileOps::CloseFD(pdev->pci_config)) {
    return -1;
  }

  for (int i = 0; i < kMaxPciBars; i++) {
    if (pdev->bars[i]) {
      CHECK(LinuxFileOps::MUnmap(pdev->bars[i], pdev->bar_sizes_bytes[i]))
          << "Failed to unmap bar " << i;
    }
  }
  delete pdev;
  return 0;
}

int ggl_pci_cfgread16(struct ggl_pci_device *pdev, unsigned offset,
                      uint16_t *val) {
  if (offset + sizeof(uint16_t) > pdev->config_size_bytes) {
    return -1;
  }
  uint16_t little_endian_value;
  size_t bytes_read =
      pread(pdev->pci_config, &little_endian_value, sizeof(uint16_t), offset);
  if (bytes_read != sizeof(uint16_t)) {
    return -1;
  }
  *val = le16toh(little_endian_value);
  return 0;
}

int ggl_pci_cfgwrite16(struct ggl_pci_device *pdev, unsigned offset,
                       uint16_t val) {
  if (offset + sizeof(uint16_t) > pdev->config_size_bytes) {
    return -1;
  }
  uint16_t little_endian_value = htole16(val);
  size_t bytes_written =
      pwrite(pdev->pci_config, &little_endian_value, sizeof(uint16_t), offset);
  return (bytes_written == sizeof(uint16_t)) ? 0 : -1;
}

int ggl_pci_cfgread32(struct ggl_pci_device *pdev, unsigned offset,
                      uint32_t *val) {
  if (offset + sizeof(uint32_t) > pdev->config_size_bytes) {
    return -1;
  }
  uint32_t little_endian_value;
  size_t bytes_read =
      pread(pdev->pci_config, &little_endian_value, sizeof(uint32_t), offset);
  if (bytes_read != sizeof(uint32_t)) {
    return -1;
  }
  *val = le32toh(little_endian_value);
  return 0;
}

int ggl_pci_cfgwrite32(struct ggl_pci_device *pdev, unsigned offset,
                       uint32_t val) {
  if (offset + sizeof(uint32_t) > pdev->config_size_bytes) {
    return -1;
  }
  uint32_t little_endian_value = htole32(val);
  size_t bytes_written =
      pwrite(pdev->pci_config, &little_endian_value, sizeof(uint32_t), offset);
  return (bytes_written == sizeof(uint32_t)) ? 0 : -1;
}

int ggl_pci_mread32(struct ggl_pci_device *pdev, unsigned bar_index,
                    uint64_t offset, uint32_t *val) {
  if (pdev->bars[bar_index] == nullptr ||
      offset + sizeof(uint32_t) > pdev->bar_sizes_bytes[bar_index]) {
    return -1;
  }
  memcpy(val, pdev->bars[bar_index] + offset, sizeof(uint32_t));
  return 0;
}

int ggl_pci_mwrite32(struct ggl_pci_device *pdev, unsigned bar_index,
                     uint64_t offset, uint32_t val) {
  if (pdev->bars[bar_index] == nullptr ||
      offset + sizeof(uint32_t) > pdev->bar_sizes_bytes[bar_index]) {
    return -1;
  }
  memcpy(pdev->bars[bar_index] + offset, &val, sizeof(uint32_t));
  return 0;
}

int ggl_pci_mpoll32(struct ggl_pci_device *pdev, unsigned bar_index,
                    unsigned bar_offset, uint32_t mask, uint32_t expected_val,
                    unsigned timeout_usecs, uint32_t *last_val) {
  // Validate expected_val.
  if ((expected_val & mask) != expected_val) {
    LOG(ERROR) << "Invalid expected_val and mask passed to ggl_pci_mpoll32().";
    return -1;
  }

  const absl::Time deadline = absl::Now() + absl::Microseconds(timeout_usecs);
  uint32_t val;
  while (true) {
    const int result = ggl_pci_mread32(pdev, bar_index, bar_offset, &val);
    if (result != 0) {
      return -1;
    }
    if ((val & mask) == expected_val) {
      if (last_val != nullptr) {
        *last_val = val;
      }
      return 0;
    }
    if (absl::Now() > deadline) {
      return 1;
    }
    // Yield to the kernel to reduce CPU usage.
    sched_yield();
  }
}
