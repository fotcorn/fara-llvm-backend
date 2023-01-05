//===-- FARAISelLowering.cpp - FARA DAG Lowering Implementation  ----------===//
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

#include "FARAISelLowering.h"
#include "FARA.h"
#include "FARASubtarget.h"
#include "llvm/CodeGen/CallingConvLower.h"

using namespace llvm;

#define DEBUG_TYPE "fara-lower"

FARATargetLowering::FARATargetLowering(const TargetMachine &TM,
                                       const FARASubtarget &STI)
    : TargetLowering(TM) {
  addRegisterClass(MVT::i64, &FARA::IntRegsRegClass);
  computeRegisterProperties(STI.getRegisterInfo());
}

SDValue FARATargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool /*isVarArg*/,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc & /*dl*/,
    SelectionDAG & /*DAG*/, SmallVectorImpl<SDValue> &InVals) const {

  if (CallConv != CallingConv::C) {
    report_fatal_error("Unsupported calling convention");
  }

  if (Ins.size() != 0) {
    report_fatal_error("Function arguments not yet supported");
  }

  if (InVals.size() != 0) {
    report_fatal_error("Function arguments not yet supported");
  }

  return Chain;
}

#include "FARAGenCallingConv.inc"

SDValue FARATargetLowering::LowerReturn(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::OutputArg> & Outs,
    const SmallVectorImpl<SDValue> & OutVals, const SDLoc & DL,
    SelectionDAG & DAG) const {
  // CCValAssign - represent the assignment of the return value to a location
  SmallVector<CCValAssign, 16> RVLocs;

  // CCState - Info about the registers and stack slot.
  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());

  // Analize return values.
  CCInfo.AnalyzeReturn(Outs, RetCC_FARA);

  SDValue Flag;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  // Copy the result values into the output registers.
  for (unsigned i = 0; i != RVLocs.size(); ++i) {
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers!");

    Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), OutVals[i], Flag);

    // Guarantee that all emitted copies are stuck together with flags.
    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }

  RetOps[0] = Chain; // Update chain.

  // Add the flag if we have it.
  if (Flag.getNode())
    RetOps.push_back(Flag);

  return DAG.getNode(FARAISD::RET_FLAG, DL, MVT::Other, RetOps);
}
