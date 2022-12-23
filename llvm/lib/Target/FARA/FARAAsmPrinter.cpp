//===- FARAAsmPrinter.cpp - FARA LLVM assembly writer ------------*- C++-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to GNU format FARA assembly language.
//
//===----------------------------------------------------------------------===//

#include "FARA.h"
#include "FARASubtarget.h"
#include "FARATargetMachine.h"
#include "TargetInfo/FARATargetInfo.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "fara-asmprinter"

namespace {

class FARAAsmPrinter : public AsmPrinter {
public:
  explicit FARAAsmPrinter(TargetMachine &TM,
                          std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)) {}

  StringRef getPassName() const override { return "FARA Assembly Printer"; }
  void emitInstruction(const MachineInstr *MI) override;
};

} // end anonymous namespace

void FARAAsmPrinter::emitInstruction(const MachineInstr *MI) {
  FARA_MC::verifyInstructionPredicates(MI->getOpcode(),
                                       getSubtargetInfo().getFeatureBits());

  switch (MI->getOpcode()) {
  case FARA::DBG_VALUE:
    llvm_unreachable("Should be handled target independently");
    break;
  }

  MCInst TmpInst;
  lowerFARAMachineInstrToMCInst(MI, TmpInst, *this);
  EmitToStreamer(*OutStreamer, TmpInst);
}

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeFARAAsmPrinter() {
  RegisterAsmPrinter<FARAAsmPrinter> X(getTheFARATarget());
}
