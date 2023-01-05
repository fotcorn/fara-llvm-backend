//===-- FARAISelLowering.h - FARA DAG Lowering Interface -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that FARA uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_FARA_FARAISELLOWERING_H
#define LLVM_LIB_TARGET_FARA_FARAISELLOWERING_H

#include "FARA.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {
class FARASubtarget;

namespace FARAISD {
enum NodeType : unsigned {
  // Start the numbering from where ISD NodeType finishes.
  FIRST_NUMBER = ISD::BUILTIN_OP_END,
  RET_FLAG,
};
}

class FARATargetLowering : public TargetLowering {
public:
  explicit FARATargetLowering(const TargetMachine &TM,
                              const FARASubtarget &STI);

  SDValue
  LowerFormalArguments(SDValue /*Chain*/, CallingConv::ID /*CallConv*/,
                       bool /*isVarArg*/,
                       const SmallVectorImpl<ISD::InputArg> & /*Ins*/,
                       const SDLoc & /*dl*/, SelectionDAG & /*DAG*/,
                       SmallVectorImpl<SDValue> & /*InVals*/) const override;

  SDValue LowerReturn(SDValue /*Chain*/, CallingConv::ID /*CallConv*/,
                      bool /*isVarArg*/,
                      const SmallVectorImpl<ISD::OutputArg> & /*Outs*/,
                      const SmallVectorImpl<SDValue> & /*OutVals*/,
                      const SDLoc & /*dl*/,
                      SelectionDAG & /*DAG*/) const override;
};
} // namespace llvm

#endif
