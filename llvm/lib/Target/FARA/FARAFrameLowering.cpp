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
#include "FARA.h"
#include "FARAInstrInfo.h"
#include "FARASubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

void FARAFrameLowering::emitPrologue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.begin();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  const FARAInstrInfo &TII = *STI.getInstrInfo();
  DebugLoc DL;

  if (hasFP(MF)) {
    // push %fp
    BuildMI(MBB, MBBI, DL, TII.get(FARA::PUSH8_r)).addReg(FARA::FP);
    // mov %sp, %fp
    BuildMI(MBB, MBBI, DL, TII.get(FARA::MOV8_rr))
        .addReg(FARA::FP)
        .addReg(FARA::SP);
    // sub <stack size>, %sp
    BuildMI(MBB, MBBI, DL, TII.get(FARA::ADD8S_ir), FARA::SP)
        .addImm(-MFI.getStackSize())
        .addReg(FARA::SP);
  }
}
void FARAFrameLowering::emitEpilogue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  const FARAInstrInfo &TII = *STI.getInstrInfo();
  DebugLoc DL;

  if (hasFP(MF)) {
    // mov %fp, %sp
    BuildMI(MBB, MBBI, DL, TII.get(FARA::MOV8_rr))
        .addReg(FARA::SP)
        .addReg(FARA::FP);
    // pop %fp
    BuildMI(MBB, MBBI, DL, TII.get(FARA::POP8_r)).addReg(FARA::FP);
  }
}

// Return true if the specified function should have a dedicated frame
// pointer register.  This is true if frame pointer elimination is
// disabled, if it needs dynamic stack realignment, if the function has
// variable sized allocas, or if the frame address is taken.
bool FARAFrameLowering::hasFP(const MachineFunction &MF) const {
  const TargetRegisterInfo *RegInfo = STI.getRegisterInfo();

  const MachineFrameInfo &MFI = MF.getFrameInfo();
  return MF.getTarget().Options.DisableFramePointerElim(MF) ||
         RegInfo->hasStackRealignment(MF) || MFI.hasVarSizedObjects() ||
         MFI.isFrameAddressTaken();
}

MachineBasicBlock::iterator FARAFrameLowering::eliminateCallFramePseudoInstr(
    MachineFunction &MF, MachineBasicBlock &MBB,
    MachineBasicBlock::iterator I) const {
  if (!hasReservedCallFrame(MF)) {
    MachineInstr &MI = *I;
    int64_t Size = MI.getOperand(0).getImm();
    if (MI.getOpcode() == FARA::ADJCALLSTACKDOWN)
      Size = -Size;

    if (Size)
      emitSPAdjustment(MF, MBB, I, Size);
  }
  return MBB.erase(I);
}

void FARAFrameLowering::emitSPAdjustment(MachineFunction &MF,
                                       MachineBasicBlock &MBB,
                                       MachineBasicBlock::iterator MBBI,
                                       int64_t NumBytes) const {
  DebugLoc DL;
  const FARAInstrInfo &TII = *STI.getInstrInfo();

  if (NumBytes == 0) {
    return;
  }

  BuildMI(MBB, MBBI, DL, TII.get(FARA::ADD8S_ir), FARA::SP)
      .addImm(NumBytes);
}
