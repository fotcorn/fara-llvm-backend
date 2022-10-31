//===-- FARASubtarget.h - Define Subtarget for the FARA -------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the FARA implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_FARA_FARAINSTRINFO_H
#define LLVM_LIB_TARGET_FARA_FARAINSTRINFO_H

#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "FARAGenInstrInfo.inc"

namespace llvm {

class FARASubtarget;

class FARAInstrInfo : public FARAGenInstrInfo {
public:
  explicit FARAInstrInfo(FARASubtarget &ST);
};

} // namespace llvm

#endif
