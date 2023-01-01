//===-- FARARegisterInfo.cpp - FARA Register Information ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the FARA implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "FARARegisterInfo.h"
#include "FARA.h"
#include "FARASubtarget.h"
#include "llvm/CodeGen/MachineFunction.h"

#define GET_REGINFO_TARGET_DESC
#include "FARAGenRegisterInfo.inc"

using namespace llvm;

FARARegisterInfo::FARARegisterInfo()
    : FARAGenRegisterInfo(0, /*DwarfFlavour*/ 0, /*EHFlavor*/ 0,
                          /*PC*/ FARA::PC) {}

Register FARARegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  const FARAFrameLowering *TFI = getFrameLowering(MF);
  return TFI->hasFP(MF) ? FARA::FP : FARA::SP;
}

const MCPhysReg *
FARARegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_SaveList;
}

BitVector FARARegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  markSuperRegs(Reserved, FARA::PC);
  markSuperRegs(Reserved, FARA::SP);
  markSuperRegs(Reserved, FARA::FP);
  return Reserved;
}

bool FARARegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator MI,
                                           int SPAdj, unsigned FIOperandNum,
                                           RegScavenger *RS) const {
  llvm_unreachable("FARARegisterInfo::eliminateFrameIndex not implemented");
  return false;
}
