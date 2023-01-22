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
#include "MCTargetDesc/FARAMCExpr.h"
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

static MCOperand lowerSymbolOperand(const MachineInstr *MI,
                                    const MachineOperand &MO, AsmPrinter &AP) {
  FARAMCExpr::VariantKind Kind = (FARAMCExpr::VariantKind)MO.getTargetFlags();
  const MCSymbol *Symbol = nullptr;

  switch (MO.getType()) {
  default:
    llvm_unreachable("Unknown type in lowerSymbolOperand");
  case MachineOperand::MO_MachineBasicBlock:
    Symbol = MO.getMBB()->getSymbol();
    break;

  case MachineOperand::MO_GlobalAddress:
    Symbol = AP.getSymbol(MO.getGlobal());
    break;
  }

  const MCSymbolRefExpr *MCSym = MCSymbolRefExpr::create(Symbol, AP.OutContext);
  const FARAMCExpr *expr = FARAMCExpr::create(MCSym, Kind, AP.OutContext);
  return MCOperand::createExpr(expr);
}

static MCOperand lowerOperand(const MachineInstr *MI, const MachineOperand &MO,
                              AsmPrinter &AP) {
  switch (MO.getType()) {
  default:
    report_fatal_error("unsupported operand type");

  case MachineOperand::MO_Register:
    if (MO.isImplicit())
      break;
    return MCOperand::createReg(MO.getReg());
  case MachineOperand::MO_GlobalAddress:
    return lowerSymbolOperand(MI, MO, AP);
  case MachineOperand::MO_Immediate:
    return MCOperand::createImm(MO.getImm());
  case MachineOperand::MO_RegisterMask:
    break;
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
