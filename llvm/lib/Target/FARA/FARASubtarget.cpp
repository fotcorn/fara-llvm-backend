
//===-- FARASubtarget.cpp - FARA Subtarget Information
//------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the FARA specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "FARASubtarget.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "fara-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "FARAGenSubtargetInfo.inc"

void FARASubtarget::anchor() {}

FARASubtarget::FARASubtarget(const Triple &TT, const std::string &CPU,
                                   const std::string &FS,
                                   const TargetMachine &TM)
    : FARAGenSubtargetInfo(TT, CPU, FS) /*, InstrInfo(),
      FrameLowering(*this), TLInfo(TM, *this)*/
{}
