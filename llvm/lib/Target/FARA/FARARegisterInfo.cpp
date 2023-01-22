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
#include "llvm/CodeGen/MachineFrameInfo.h"
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

const uint32_t *
FARARegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID CC) const {
  return CSR_RegMask;
}

BitVector FARARegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  markSuperRegs(Reserved, FARA::PC);
  markSuperRegs(Reserved, FARA::SP);
  markSuperRegs(Reserved, FARA::FP);
  return Reserved;
}

bool FARARegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator MBBI,
                                           int SPAdj, unsigned FIOperandNum,
                                           RegScavenger *RS) const {
  MachineInstr &MI = *MBBI;
  DebugLoc dl = MI.getDebugLoc();
  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  MachineFunction &MF = *MI.getParent()->getParent();
  const FARAFrameLowering *TFI = getFrameLowering(MF);
  MachineFrameInfo &MFI = MF.getFrameInfo();

  Register FrameReg;
  int Offset;
  Offset = TFI->getFrameIndexReference(MF, FrameIndex, FrameReg).getFixed();
  Offset += MI.getOperand(FIOperandNum + 1).getImm();

  // if we have a frame pointer, we use the offset from there
  if (TFI->hasFP(MF)) {
    Offset = -MFI.getStackSize() + Offset;
  }

  MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, false);
  MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);

  // we updated the instruction, not replaced it
  return false;
}
