
//===-- MachineSubtarget.cpp - Machine Subtarget Information
//------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the Machine specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "MachineSubtarget.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "machine-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "MachineGenSubtargetInfo.inc"

void MachineSubtarget::anchor() {}

MachineSubtarget::MachineSubtarget(const Triple &TT, const std::string &CPU,
                                   const std::string &FS,
                                   const TargetMachine &TM)
    : MachineGenSubtargetInfo(TT, CPU, FS) /*, InstrInfo(),
      FrameLowering(*this), TLInfo(TM, *this)*/
{}
