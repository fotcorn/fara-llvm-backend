//===-- FARATargetInfo.cpp - FARA Target Implementation -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/FARATargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
using namespace llvm;

Target &llvm::getTheFARATarget() {
  static Target TheFARATarget;
  return TheFARATarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeFARATargetInfo() {
  RegisterTarget<Triple::fara, /*HasJIT=*/false> X(
      getTheFARATarget(), "fara", "FARA", "FARA");
}
