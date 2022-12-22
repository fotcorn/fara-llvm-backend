#ifndef LLVM_LIB_TARGET_FARA_FARATARGETMACHINE_H
#define LLVM_LIB_TARGET_FARA_FARATARGETMACHINE_H

#include "FARASubtarget.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class FARATargetMachine : public LLVMTargetMachine {
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

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }
  */
};
} // namespace llvm

#endif /* LLVM_LIB_TARGET_FARA_FARATARGETMACHINE_H */
