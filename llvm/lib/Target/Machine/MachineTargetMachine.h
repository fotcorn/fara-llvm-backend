#ifndef LLVM_LIB_TARGET_MACHINE_MACHINETARGETMACHINE_H
#define LLVM_LIB_TARGET_MACHINE_MACHINETARGETMACHINE_H

#include "MachineSubtarget.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class MachineTargetMachine : public LLVMTargetMachine {
  MachineSubtarget Subtarget;
  std::unique_ptr<TargetLoweringObjectFile> TLOF;

public:
  MachineTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                       StringRef FS, const TargetOptions &Options,
                       Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                       CodeGenOpt::Level OL, bool JIT);

  const TargetSubtarget *
  getSubtargetImpl(const llvm::Function & /*Fn*/) const override {
    return &Subtarget;
  }

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetTransformInfo getTargetTransformInfo(const Function &F) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }
};
} // namespace llvm

#endif /* LLVM_LIB_TARGET_MACHINE_MACHINETARGETMACHINE_H */
