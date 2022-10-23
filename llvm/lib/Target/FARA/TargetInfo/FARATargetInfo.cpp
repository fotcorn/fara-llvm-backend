//===-- VETargetInfo.cpp - VE Target Implementation -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/FARATargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheFARATarget() {
  static Target TheFARATarget;
  return TheFARATarget;
}

extern "C" void LLVMInitializeVETargetInfo() {
  RegisterTarget<Triple::fara, /*HasJIT=*/false> X(
      getTheFARATarget(), "fara", "FARA", "FARA");
}
