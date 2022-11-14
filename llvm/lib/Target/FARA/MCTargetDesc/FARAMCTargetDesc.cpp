//===-- FARAMCTargetDesc.cpp - FARA Target Descriptions -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// This file provides FARA-specific target descriptions.
///
//===----------------------------------------------------------------------===//

#include "FARAMCTargetDesc.h"
//#include "FARAELFStreamer.h"
//#include "FARAInstPrinter.h"
#include "FARAMCAsmInfo.h"
//#include "FARAMCObjectFileInfo.h"
#include "FARATargetStreamer.h"
#include "TargetInfo/FARATargetInfo.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCObjectFileInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"

#define GET_INSTRINFO_MC_DESC
#define ENABLE_INSTR_PREDICATE_VERIFIER
#include "FARAGenInstrInfo.inc"

#define GET_REGINFO_MC_DESC
#include "FARAGenRegisterInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "FARAGenSubtargetInfo.inc"

using namespace llvm;

static MCInstrInfo *createFARAMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitFARAMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createFARAMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitFARAMCRegisterInfo(X, FARA::PC);
  return X;
}

static MCAsmInfo *createFARAMCAsmInfo(const MCRegisterInfo &MRI,
                                      const Triple &TT,
                                      const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new FARAELFMCAsmInfo(TT);

  MCRegister SP = MRI.getDwarfRegNum(FARA::SP, true);
  MCCFIInstruction Inst = MCCFIInstruction::cfiDefCfa(nullptr, SP, 0);
  MAI->addInitialFrameState(Inst);

  return MAI;
}
/*
static MCObjectFileInfo *
createFARAMCObjectFileInfo(MCContext &Ctx, bool PIC,
                            bool LargeCodeModel = false) {
  MCObjectFileInfo *MOFI = new FARAMCObjectFileInfo();
  MOFI->initMCObjectFileInfo(Ctx, PIC, LargeCodeModel);
  return MOFI;
}
*/

static MCSubtargetInfo *createFARAMCSubtargetInfo(const Triple &TT,
                                                  StringRef CPU, StringRef FS) {
  return createFARAMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

/*
static MCInstPrinter *createFARAMCInstPrinter(const Triple &T,
                                               unsigned SyntaxVariant,
                                               const MCAsmInfo &MAI,
                                               const MCInstrInfo &MII,
                                               const MCRegisterInfo &MRI) {
  return new FARAInstPrinter(MAI, MII, MRI);
}

static MCTargetStreamer *
createFARAObjectTargetStreamer(MCStreamer &S, const MCSubtargetInfo &STI) {
  const Triple &TT = STI.getTargetTriple();
  if (TT.isOSBinFormatELF())
    return new FARATargetELFStreamer(S, STI);
  return nullptr;
}
*/

static MCTargetStreamer *createFARAAsmTargetStreamer(MCStreamer &S,
                                                     formatted_raw_ostream &OS,
                                                     MCInstPrinter *InstPrint,
                                                     bool isVerboseAsm) {
  return new FARATargetAsmStreamer(S, OS);
}

static MCTargetStreamer *createFARANullTargetStreamer(MCStreamer &S) {
  return new FARATargetStreamer(S);
}

/*
static MCInstrAnalysis *createFARAInstrAnalysis(const MCInstrInfo *Info) {
  return new FARAMCInstrAnalysis(Info);
}
*/

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeFARATargetMC() {
  for (Target *T : {&getTheFARATarget()}) {
    TargetRegistry::RegisterMCAsmInfo(*T, createFARAMCAsmInfo);
    // TargetRegistry::RegisterMCObjectFileInfo(*T, createFARAMCObjectFileInfo);
    TargetRegistry::RegisterMCInstrInfo(*T, createFARAMCInstrInfo);
    TargetRegistry::RegisterMCRegInfo(*T, createFARAMCRegisterInfo);
    TargetRegistry::RegisterMCAsmBackend(*T, createFARAAsmBackend);
    TargetRegistry::RegisterMCCodeEmitter(*T, createFARAMCCodeEmitter);
    // TargetRegistry::RegisterMCInstPrinter(*T, createFARAMCInstPrinter);
    TargetRegistry::RegisterMCSubtargetInfo(*T, createFARAMCSubtargetInfo);
    // TargetRegistry::RegisterELFStreamer(*T, createFARAELFStreamer);
    // TargetRegistry::RegisterMCInstrAnalysis(*T, createFARAInstrAnalysis);

    //TargetRegistry::RegisterObjectTargetStreamer(
    //    *T, createFARAObjectTargetStreamer);
    TargetRegistry::RegisterAsmTargetStreamer(*T, createFARAAsmTargetStreamer);
    TargetRegistry::RegisterNullTargetStreamer(*T,
                                               createFARANullTargetStreamer);
  }
}
