//===- FARAMCAsmInfo.cpp - FARA asm properties --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the FARAMCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "FARAMCAsmInfo.h"
#include "llvm/ADT/Triple.h"
#include "llvm/BinaryFormat/Dwarf.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCTargetOptions.h"

using namespace llvm;

void FARAELFMCAsmInfo::anchor() {}

FARAELFMCAsmInfo::FARAELFMCAsmInfo(const Triple &TheTriple) {
  CodePointerSize = CalleeSaveStackSlotSize = 8;
  SupportsDebugInformation = true;
}
