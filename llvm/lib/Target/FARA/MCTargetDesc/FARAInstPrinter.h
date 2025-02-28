//===-- FARAInstPrinter.h - Convert FARA MCInst to assembly syntax --------===//
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

#ifndef LLVM_LIB_TARGET_FARA_INSTPRINTER_FARAINSTPRINTER_H
#define LLVM_LIB_TARGET_FARA_INSTPRINTER_FARAINSTPRINTER_H

#include "FARAMCTargetDesc.h"
#include "llvm/MC/MCInstPrinter.h"

namespace llvm {

class FARAInstPrinter : public MCInstPrinter {
public:
  FARAInstPrinter(const MCAsmInfo &MAI, const MCInstrInfo &MII,
                  const MCRegisterInfo &MRI)
      : MCInstPrinter(MAI, MII, MRI) {}

  void printRegName(raw_ostream &OS, MCRegister Reg) const override;
  void printInst(const MCInst *MI, uint64_t Address, StringRef Annot,
                 const MCSubtargetInfo &STI, raw_ostream &OS) override;
  void printOperand(const MCInst *MI, int OpNum, const MCSubtargetInfo &STI,
                    raw_ostream &OS);
  void printBranchTarget(const MCInst *MI, uint64_t Address, int OpNum,
                         const MCSubtargetInfo &STI, raw_ostream &OS);
  void printMemOperand(const MCInst *MI, int OpNum, const MCSubtargetInfo &STI,
                       raw_ostream &OS);
  // Autogenerated by tblgen.
  std::pair<const char *, uint64_t> getMnemonic(const MCInst *MI) override;
  bool printAliasInstr(const MCInst *, uint64_t Address,
                       const MCSubtargetInfo &, raw_ostream &);
  void printInstruction(const MCInst *, uint64_t, const MCSubtargetInfo &,
                        raw_ostream &);
  static const char *getRegisterName(MCRegister RegNo);
};
} // namespace llvm

#endif
