//===- FARAMCAsmInfo.h - FARA asm properties -----------------------*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the FARAMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_FARA_MCTARGETDESC_FARAMCASMINFO_H
#define LLVM_LIB_TARGET_FARA_MCTARGETDESC_FARAMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {

class Triple;

class FARAELFMCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit FARAELFMCAsmInfo(const Triple &TheTriple);
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_FARA_MCTARGETDESC_FARAMCASMINFO_H
