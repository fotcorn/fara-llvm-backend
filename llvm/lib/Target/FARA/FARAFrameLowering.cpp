//===-- FARAFrameLowering.cpp - FARA Frame Information -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the FARA implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "FARAFrameLowering.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

void FARAFrameLowering::emitPrologue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {}
void FARAFrameLowering::emitEpilogue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {}

// Return true if the specified function should have a dedicated frame
// pointer register.  This is true if frame pointer elimination is
// disabled, if it needs dynamic stack realignment, if the function has
// variable sized allocas, or if the frame address is taken.
bool FARAFrameLowering::hasFP(const MachineFunction &MF) const {
  const TargetRegisterInfo *RegInfo = MF.getSubtarget().getRegisterInfo();

  const MachineFrameInfo &MFI = MF.getFrameInfo();
  return MF.getTarget().Options.DisableFramePointerElim(MF) ||
         RegInfo->hasStackRealignment(MF) || MFI.hasVarSizedObjects() ||
         MFI.isFrameAddressTaken();
}
