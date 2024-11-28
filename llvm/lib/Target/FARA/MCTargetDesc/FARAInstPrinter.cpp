//===-- FARAInstPrinter.cpp - Convert FARA MCInst to assembly syntax -------==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This class prints an FARA MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "FARAInstPrinter.h"
#include "FARA.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "fara-asmprinter"

#define GET_INSTRUCTION_NAME
#define PRINT_ALIAS_INSTR
#include "FARAGenAsmWriter.inc"

void FARAInstPrinter::printRegName(raw_ostream &OS, MCRegister Reg) const {
  OS << '%' << StringRef(getRegisterName(Reg)).lower();
}

void FARAInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                StringRef Annot, const MCSubtargetInfo &STI,
                                raw_ostream &OS) {
  if (!printAliasInstr(MI, Address, STI, OS))
    printInstruction(MI, Address, STI, OS);
  printAnnotation(OS, Annot);
}

void FARAInstPrinter::printOperand(const MCInst *MI, int OpNum,
                                   const MCSubtargetInfo &STI,
                                   raw_ostream &OS) {
  const MCOperand &MO = MI->getOperand(OpNum);

  if (MO.isReg()) {
    printRegName(OS, MO.getReg());
    return;
  }

  if (MO.isImm()) {
    OS << MO.getImm();
    return;
  }

  assert(MO.isExpr() && "Unknown operand kind in printOperand");
  MO.getExpr()->print(OS, &MAI);
}

void FARAInstPrinter::printBranchTarget(const MCInst *MI, uint64_t Address,
                                        int OpNum, const MCSubtargetInfo &STI,
                                        raw_ostream &OS) {
  const MCOperand &MO = MI->getOperand(OpNum);
  if (!MO.isImm()) {
    printOperand(MI, OpNum, STI, OS);
    return;
  }
  if (PrintBranchImmAsAddress) {
    uint64_t Target = Address + MO.getImm();
    OS << formatHex(Target);
  } else {
    OS << MO.getImm();
  }
}

void FARAInstPrinter::printMemOperand(const MCInst *MI, int OpNum,
                                      const MCSubtargetInfo &STI,
                                      raw_ostream &OS) {
  printOperand(MI, OpNum, STI, OS);
  OS << ", ";
  printOperand(MI, OpNum + 1, STI, OS);
}
