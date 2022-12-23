//===-- FARAMCInstLower.cpp - Convert FARA MachineInstr to an MCInst -------=//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains code to lower FARA MachineInstrs to their corresponding
// MCInst records.
//
//===----------------------------------------------------------------------===//

#include "FARA.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

static MCOperand lowerOperand(const MachineInstr *MI, const MachineOperand &MO,
                              AsmPrinter &AP) {
  switch (MO.getType()) {
  default:
    report_fatal_error("unsupported operand type");

  case MachineOperand::MO_Register:
    if (MO.isImplicit())
      break;
    return MCOperand::createReg(MO.getReg());

  //case MachineOperand::MO_BlockAddress:
  //  return LowerSymbolOperand(
  //      MI, MO, AP.GetBlockAddressSymbol(MO.getBlockAddress()), AP);
  //case MachineOperand::MO_ConstantPoolIndex:
  //  return LowerSymbolOperand(MI, MO, AP.GetCPISymbol(MO.getIndex()), AP);
  //case MachineOperand::MO_ExternalSymbol:
  //  return LowerSymbolOperand(
  //      MI, MO, AP.GetExternalSymbolSymbol(MO.getSymbolName()), AP);
  //case MachineOperand::MO_GlobalAddress:
  //  return LowerSymbolOperand(MI, MO, AP.getSymbol(MO.getGlobal()), AP);
  case MachineOperand::MO_Immediate:
    return MCOperand::createImm(MO.getImm());
  //case MachineOperand::MO_JumpTableIndex:
  //  return LowerSymbolOperand(MI, MO, AP.GetJTISymbol(MO.getIndex()), AP);
  //case MachineOperand::MO_MachineBasicBlock:
  //  return LowerSymbolOperand(MI, MO, MO.getMBB()->getSymbol(), AP);

  //case MachineOperand::MO_RegisterMask:
  //  break;
  }
  return MCOperand();
}

void llvm::lowerFARAMachineInstrToMCInst(const MachineInstr *MI, MCInst &OutMI,
                                   AsmPrinter &AP) {
  OutMI.setOpcode(MI->getOpcode());

  for (const MachineOperand &MO : MI->operands()) {
    MCOperand MCOp = lowerOperand(MI, MO, AP);

    if (MCOp.isValid())
      OutMI.addOperand(MCOp);
  }
}
