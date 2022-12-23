//===-- FARATargetMachine.cpp - Define TargetMachine for FARA -------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#include "FARATargetMachine.h"
#include "FARA.h"
#include "TargetInfo/FARATargetInfo.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

#define DEBUG_TYPE "fara"

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeFARATarget() {
  // Register the target.
  RegisterTargetMachine<FARATargetMachine> X(getTheFARATarget());
}

static std::string computeDataLayout() {
  // little endian
  std::string Ret = "e";

  // ELF mangling
  Ret += "-m:e";

  // 64 bit integers alignment is 64bit.
  Ret += "-i64:64";

  // 64 bit registers
  Ret += "-n64";

  // Stack alignment is 128 bits
  Ret += "-S128";

  return Ret;
}

static Reloc::Model getEffectiveRelocModel(std::optional<Reloc::Model> RM) {
  return RM.value_or(Reloc::Static);
}

class FARAELFTargetObjectFile : public TargetLoweringObjectFileELF {
  void Initialize(MCContext &Ctx, const TargetMachine &TM) override {
    TargetLoweringObjectFileELF::Initialize(Ctx, TM);
    InitializeELF(TM.Options.UseInitArray);
  }
};

static std::unique_ptr<TargetLoweringObjectFile> createTLOF() {
  return std::make_unique<FARAELFTargetObjectFile>();
}

FARATargetMachine::FARATargetMachine(const Target &T, const Triple &TT,
                                     StringRef CPU, StringRef FS,
                                     const TargetOptions &Options,
                                     std::optional<Reloc::Model> RM,
                                     std::optional<CodeModel::Model> CM,
                                     CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(), TT, CPU, FS, Options,
                        getEffectiveRelocModel(RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(createTLOF()),
      Subtarget(TT, std::string(CPU), std::string(FS), *this) {
  initAsmInfo();
}

namespace {
class FARAPassConfig final : public TargetPassConfig {
public:
  FARAPassConfig(FARATargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  FARATargetMachine &getFARATargetMachine() const {
    return getTM<FARATargetMachine>();
  }

  bool addInstSelector() override;
};

bool FARAPassConfig::addInstSelector() {
  addPass(createFARAISelDag(getFARATargetMachine()));
  return false;
}

} // end anonymous namespace

TargetPassConfig *FARATargetMachine::createPassConfig(PassManagerBase &PM) {
  return new FARAPassConfig(*this, PM);
}
