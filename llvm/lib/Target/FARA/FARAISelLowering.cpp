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
#include "llvm/CodeGen/MachineFrameInfo.h"

using namespace llvm;

#define DEBUG_TYPE "fara-lower"

FARATargetLowering::FARATargetLowering(const TargetMachine &TM,
                                       const FARASubtarget &STI)
    : TargetLowering(TM), Subtarget(&STI) {
  addRegisterClass(MVT::i64, &FARA::IntRegsRegClass);
  computeRegisterProperties(STI.getRegisterInfo());

  setOperationAction(ISD::BR_CC, MVT::i64, Expand); // expand br_cc to brcond

  setOperationAction(ISD::GlobalAddress, MVT::i64, Custom);
}

const char *FARATargetLowering::getTargetNodeName(unsigned Opcode) const {
#define NODE_NAME_CASE(NODE)                                                   \
  case FARAISD::NODE:                                                          \
    return "FARAISD::" #NODE;
  switch ((FARAISD::NodeType)Opcode) {
  case FARAISD::FIRST_NUMBER:
    break;
    NODE_NAME_CASE(RET_FLAG)
    NODE_NAME_CASE(CALL)
  }
  return nullptr;
#undef NODE_NAME_CASE
}

#include "FARAGenCallingConv.inc"

SDValue FARATargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {

  if (CallConv != CallingConv::C) {
    report_fatal_error("Unsupported calling convention");
  }
  if (IsVarArg) {
    report_fatal_error("Vararg is not supported");
  }

  MachineFunction &MF = DAG.getMachineFunction();
  // Assign locations to all of the incoming arguments.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, IsVarArg, MF, ArgLocs, *DAG.getContext());

  CCInfo.AnalyzeFormalArguments(Ins, CC_FARA);

  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    CCValAssign &VA = ArgLocs[i];

    if (VA.isRegLoc()) {
      // This argument is passed in a register.
      // All integer register arguments are promoted by the caller to i64.

      // Create a virtual register for the promoted live-in value.
      Register VReg =
          MF.addLiveIn(VA.getLocReg(), getRegClassFor(VA.getLocVT()));
      SDValue Arg = DAG.getCopyFromReg(Chain, DL, VReg, VA.getLocVT());

      // The caller promoted the argument, so insert an Assert?ext SDNode so we
      // won't promote the value again in this function.
      switch (VA.getLocInfo()) {
      case CCValAssign::SExt:
        Arg = DAG.getNode(ISD::AssertSext, DL, VA.getLocVT(), Arg,
                          DAG.getValueType(VA.getValVT()));
        break;
      case CCValAssign::ZExt:
        Arg = DAG.getNode(ISD::AssertZext, DL, VA.getLocVT(), Arg,
                          DAG.getValueType(VA.getValVT()));
        break;
      case CCValAssign::Full:
        break;
      default:
        report_fatal_error("CCValAssign type not supported");
      }

      // Truncate the register down to the argument type.
      if (VA.isExtInLoc())
        Arg = DAG.getNode(ISD::TRUNCATE, DL, VA.getValVT(), Arg);

      InVals.push_back(Arg);
      continue;
    }

    // Only arguments pased on the stack should make it here.
    assert(VA.isMemLoc());

    // todo: maybe we need some additional offset here, ABI is not yet fully
    // defined.
    unsigned Offset = VA.getLocMemOffset();
    unsigned ValSize = VA.getValVT().getSizeInBits() / 8;

    int FI = MF.getFrameInfo().CreateFixedObject(ValSize, Offset, true);
    InVals.push_back(
        DAG.getLoad(VA.getValVT(), DL, Chain,
                    DAG.getFrameIndex(FI, getPointerTy(MF.getDataLayout())),
                    MachinePointerInfo::getFixedStack(MF, FI)));
  }

  return Chain;
}

SDValue
FARATargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                bool IsVarArg,
                                const SmallVectorImpl<ISD::OutputArg> &Outs,
                                const SmallVectorImpl<SDValue> &OutVals,
                                const SDLoc &DL, SelectionDAG &DAG) const {
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

SDValue FARATargetLowering::LowerCall(TargetLowering::CallLoweringInfo &CLI,
                                      SmallVectorImpl<SDValue> &InVals) const {
  SelectionDAG &DAG = CLI.DAG;
  SDLoc DL = CLI.DL;
  SDValue Chain = CLI.Chain;
  auto PtrVT = getPointerTy(DAG.getDataLayout());

  // FARA does not support tail call optimization yet
  CLI.IsTailCall = false;
  assert(!CLI.IsVarArg && "FARA does not support var args yet");

  // Analyze operands of the call, assigning locations to each operand.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CLI.CallConv, CLI.IsVarArg, DAG.getMachineFunction(), ArgLocs,
                 *DAG.getContext());
  CCInfo.AnalyzeCallOperands(CLI.Outs, CC_FARA);

  // Adjust the stack pointer to make room for the arguments.
  unsigned ArgsSize = CCInfo.getNextStackOffset();
  Chain = DAG.getCALLSEQ_START(Chain, ArgsSize, 0, DL);

  // Collect the set of registers to pass to the function and their values.
  // This will be emitted as a sequence of CopyToReg nodes glued to the call
  // instruction.
  SmallVector<std::pair<Register, SDValue>, 8> RegsToPass;

  // Collect chains from all the memory opeations that copy arguments to the
  // stack. They must follow the stack pointer adjustment above and precede the
  // call instruction itself.
  SmallVector<SDValue, 8> MemOpChains;

  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    const CCValAssign &VA = ArgLocs[i];
    SDValue Arg = CLI.OutVals[i];

    // Promote the value if needed.
    switch (VA.getLocInfo()) {
    default:
      llvm_unreachable("Unknown location info!");
    case CCValAssign::Full:
      break;
    }

    if (VA.isRegLoc()) {
      RegsToPass.push_back(std::make_pair(VA.getLocReg(), Arg));
      continue;
    }

    assert(VA.isMemLoc());

    // Create a store off the stack pointer for this argument.
    SDValue StackPtr = DAG.getRegister(FARA::SP, PtrVT);
    SDValue PtrOff = DAG.getIntPtrConstant(VA.getLocMemOffset(), DL);
    PtrOff = DAG.getNode(ISD::ADD, DL, PtrVT, StackPtr, PtrOff);
    MemOpChains.push_back(
        DAG.getStore(Chain, DL, Arg, PtrOff, MachinePointerInfo()));
  }

  // Emit all stores, make sure they occur before the call.
  if (!MemOpChains.empty())
    Chain = DAG.getNode(ISD::TokenFactor, DL, MVT::Other, MemOpChains);

  // Build a sequence of CopyToReg nodes glued together with token chain and
  // glue operands which copy the outgoing args into registers. The InGlue is
  // necessary since all emitted instructions must be stuck together in order
  // to pass the live physical registers.
  SDValue InGlue;
  for (auto &Reg : RegsToPass) {
    Chain = DAG.getCopyToReg(Chain, DL, Reg.first, Reg.second, InGlue);
    InGlue = Chain.getValue(1);
  }

  SDValue Callee = CLI.Callee;

  // If the callee is a GlobalAddress/ExternalSymbol node, turn it into a
  // TargetGlobalAddress/TargetExternalSymbol node so that legalize won't
  // split it and then direct call can be matched by PseudoCALL.
  if (GlobalAddressSDNode *S = dyn_cast<GlobalAddressSDNode>(Callee)) {
    Callee = DAG.getTargetGlobalAddress(S->getGlobal(), DL, PtrVT);
  } else if (ExternalSymbolSDNode *S = dyn_cast<ExternalSymbolSDNode>(Callee)) {
    Callee = DAG.getTargetExternalSymbol(S->getSymbol(), PtrVT);
  }

  // Build the operands for the call instruction itself.
  SmallVector<SDValue, 8> Ops;
  Ops.push_back(Chain);
  Ops.push_back(Callee);
  for (unsigned i = 0, e = RegsToPass.size(); i != e; ++i) {
    Ops.push_back(DAG.getRegister(RegsToPass[i].first,
                                  RegsToPass[i].second.getValueType()));
  }

  // Add a register mask operand representing the call-preserved registers.
  const FARARegisterInfo *TRI = Subtarget->getRegisterInfo();
  const uint32_t *Mask =
      TRI->getCallPreservedMask(DAG.getMachineFunction(), CLI.CallConv);
  assert(Mask && "Missing call preserved mask for calling convention");
  Ops.push_back(DAG.getRegisterMask(Mask));

  // Make sure the CopyToReg nodes are glued to the call instruction which
  // consumes the registers.
  if (InGlue.getNode())
    Ops.push_back(InGlue);

  // Emit the call.
  SDVTList NodeTys = DAG.getVTList(MVT::Other, MVT::Glue);
  Chain = DAG.getNode(FARAISD::CALL, DL, NodeTys, Ops);
  InGlue = Chain.getValue(1);

  // Revert the stack pointer immediately after the call.
  Chain = DAG.getCALLSEQ_END(Chain, ArgsSize, 0, InGlue, DL);
  InGlue = Chain.getValue(1);

  // Assign locations to each value returned by this call.
  SmallVector<CCValAssign, 16> RVLocs;
  CCState RVInfo(CLI.CallConv, CLI.IsVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());

  RVInfo.AnalyzeCallResult(CLI.Ins, RetCC_FARA);

  // Copy all of the result registers out of their specified physreg.
  for (auto &VA : RVLocs) {
    // Copy the value out
    SDValue RetValue =
        DAG.getCopyFromReg(Chain, DL, VA.getLocReg(), VA.getLocVT(), InGlue);
    // Glue the RetValue to the end of the call sequence
    Chain = RetValue.getValue(1);
    InGlue = RetValue.getValue(2);

    assert(VA.getLocInfo() == CCValAssign::Full);

    InVals.push_back(RetValue);
  }

  return Chain;
}

SDValue llvm::FARATargetLowering::LowerOperation(SDValue Op,
                                                 SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  default:
    report_fatal_error("unimplemented operand");
  case ISD::GlobalAddress:
    return lowerGlobalAddress(Op, DAG);
  }
}

SDValue FARATargetLowering::lowerGlobalAddress(SDValue Op,
                                               SelectionDAG &DAG) const {
  GlobalAddressSDNode *N = cast<GlobalAddressSDNode>(Op);
  SDLoc DL(N);
  EVT Ty = getPointerTy(DAG.getDataLayout());

  if (isPositionIndependent()) {
    llvm_unreachable("Position independen global addresses not yet handled.");
  }

  switch (getTargetMachine().getCodeModel()) {
  default:
    llvm_unreachable("Unsupported absolute code model");
  case CodeModel::Small:
  case CodeModel::Medium:
  case CodeModel::Large:
    SDValue Addr = DAG.getTargetGlobalAddress(N->getGlobal(), DL, Ty, 0);
    return SDValue(DAG.getMachineNode(FARA::MOV8_ir, DL, MVT::i64, Addr), 0);
  }
}
