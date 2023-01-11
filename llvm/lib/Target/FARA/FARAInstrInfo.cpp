//===-- FARAInstrInfo.cpp - FARA Instruction Information
//----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the FARA implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "FARAInstrInfo.h"
#include "FARA.h"
#include "FARARegisterInfo.h"
#include "llvm/CodeGen/MachineFrameInfo.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "FARAGenInstrInfo.inc"

FARAInstrInfo::FARAInstrInfo(FARASubtarget &ST) : FARAGenInstrInfo() {}

void llvm::FARAInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                                               MachineBasicBlock::iterator I,
                                               Register DestReg, int FI,
                                               const TargetRegisterClass *RC,
                                               const TargetRegisterInfo *TRI,
                                               Register VReg) const {
  DebugLoc DL;
  if (I != MBB.end())
    DL = I->getDebugLoc();

  MachineFunction *MF = MBB.getParent();
  const MachineFrameInfo &MFI = MF->getFrameInfo();
  MachineMemOperand *MMO = MF->getMachineMemOperand(
      MachinePointerInfo::getFixedStack(*MF, FI), MachineMemOperand::MOLoad,
      MFI.getObjectSize(FI), MFI.getObjectAlign(FI));

  if (RC == &FARA::AllRegsRegClass || RC == &FARA::IntRegsRegClass) {
    BuildMI(MBB, I, DL, get(FARA::LD8_rr), DestReg)
        .addFrameIndex(FI)
        .addMemOperand(MMO);
  } else {
    report_fatal_error("Can't load this register from stack slot");
  }
}
