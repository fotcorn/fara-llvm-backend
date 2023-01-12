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

void FARAInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
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
    // LDX8 %fp, imm_offset, %DestReg
    BuildMI(MBB, I, DL, get(FARA::LDX8_rir))
        .addReg(DestReg)
        .addFrameIndex(FI)
        .addImm(0)
        .addMemOperand(MMO);
  } else {
    report_fatal_error("Can't load this register from stack slot");
  }
}

void FARAInstrInfo::storeRegToStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI, Register SrcReg,
    bool isKill, int FI, const TargetRegisterClass *RC,
    const TargetRegisterInfo *TRI, Register VReg) const {
  DebugLoc DL;
  if (MBBI != MBB.end())
    DL = MBBI->getDebugLoc();

  MachineFunction *MF = MBB.getParent();
  const MachineFrameInfo &MFI = MF->getFrameInfo();
  MachineMemOperand *MMO = MF->getMachineMemOperand(
      MachinePointerInfo::getFixedStack(*MF, FI), MachineMemOperand::MOStore,
      MFI.getObjectSize(FI), MFI.getObjectAlign(FI));

  if (RC == &FARA::AllRegsRegClass || RC == &FARA::IntRegsRegClass) {
    BuildMI(MBB, MBBI, DL, get(FARA::STRX8_rri))
        .addReg(SrcReg, getKillRegState(isKill))
        .addFrameIndex(FI)
        .addImm(0)
        .addMemOperand(MMO);
  } else {
    report_fatal_error("Can't store this register to stack slot");
  }
}

void FARAInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                      MachineBasicBlock::iterator MBBI,
                                      const DebugLoc &DL, MCRegister DestReg,
                                      MCRegister SrcReg, bool KillSrc) const {
  if (FARA::AllRegsRegClass.contains(DestReg, SrcReg))
  {
    BuildMI(MBB, MBBI, DL, get(FARA::MOV8_rr), DestReg)
      .addReg(SrcReg, getKillRegState(KillSrc));
    return;
  }
  else {
    llvm_unreachable("Impossible reg-to-reg copy");
  }
}
