//===-- FARABaseInfo.h - Top level definitions for FARA MC ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains small standalone enum definitions for the FARA target
// useful for the compiler back-end and the MC libraries.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_LIB_TARGET_FARA_MCTARGETDESC_FARABASEINFO_H
#define LLVM_LIB_TARGET_FARA_MCTARGETDESC_FARABASEINFO_H

#include <cstdint>

namespace llvm {

namespace FARA {
enum {
  // see FARAInstrFormats.td for documentation for the following entries
  InstFormatNoOut = 0,
  InstFormatOneOut = 1,
  InstFormatOneOutIgnore = 2,

  InstFormatMask = 15, // inst format is 4bits
  InstFormatShift = 0,
};

// Helper functions to read TSFlags.
/// \returns the format of the instruction.
static inline unsigned getFormat(uint64_t TSFlags) {
  return (TSFlags & InstFormatMask) >> InstFormatShift;
}

}
} // namespace llvm

#endif
