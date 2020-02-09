//===-- VETargetInfo.cpp - VE Target Implementation -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/MachineTargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheMachineTarget() {
  static Target TheMachineTarget;
  return TheMachineTarget;
}

extern "C" void LLVMInitializeVETargetInfo() {
  RegisterTarget<Triple::machine, /*HasJIT=*/false> X(
      getTheMachineTarget(), "machine", "Machine", "Machine");
}
