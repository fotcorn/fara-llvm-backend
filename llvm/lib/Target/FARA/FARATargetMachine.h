//===-- FARATargetMachine.h - Define TargetMachine for FARA -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the FARA specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_FARA_FARATARGETMACHINE_H
#define LLVM_LIB_TARGET_FARA_FARATARGETMACHINE_H

#include "FARASubtarget.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class FARATargetMachine : public LLVMTargetMachine {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  FARASubtarget Subtarget;

public:
  FARATargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                    StringRef FS, const TargetOptions &Options,
                    std::optional<Reloc::Model> RM,
                    std::optional<CodeModel::Model> CM, CodeGenOpt::Level OL,
                    bool JIT);

  const FARASubtarget *getSubtargetImpl() const { return &Subtarget; }
  const FARASubtarget *getSubtargetImpl(const Function &) const override {
    return &Subtarget;
  }

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  /*
  TargetTransformInfo getTargetTransformInfo(const Function &F) override;
  */
  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }
};
} // namespace llvm

#endif /* LLVM_LIB_TARGET_FARA_FARATARGETMACHINE_H */
