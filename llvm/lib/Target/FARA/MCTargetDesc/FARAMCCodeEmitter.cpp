//===-- FARAMCCodeEmitter.cpp - Convert FARA code to machine code -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the FARAMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#include "FARABaseInfo.h"
#include "FARAMCTargetDesc.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "mccodeemitter"

STATISTIC(MCNumEmitted, "Number of MC instructions emitted");

namespace {

class FARAMCCodeEmitter : public MCCodeEmitter {
  MCInstrInfo const &MCII;

public:
  FARAMCCodeEmitter(MCContext &ctx, MCInstrInfo const &MCII) : MCII(MCII) {}
  FARAMCCodeEmitter(const FARAMCCodeEmitter &) = delete;
  FARAMCCodeEmitter &operator=(const FARAMCCodeEmitter &) = delete;
  ~FARAMCCodeEmitter() override = default;

  void encodeInstruction(const MCInst &MI, raw_ostream &OS,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;
  /// TableGen'erated function for getting the binary encoding for an
  /// instruction.
  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;
};
} // end anonymous namespace

void FARAMCCodeEmitter::encodeInstruction(const MCInst &MI, raw_ostream &OS,
                                          SmallVectorImpl<MCFixup> &Fixups,
                                          const MCSubtargetInfo &STI) const {
  uint32_t Bits = getBinaryCodeForInstr(MI, Fixups, STI);
  support::endian::write<uint32_t>(OS, Bits, support::little);

  const MCInstrDesc &Desc = MCII.get(MI.getOpcode());
  unsigned int instFormat = FARA::getFormat(Desc.TSFlags);

  // on ALU instructions, we get three operands: dst, src1, src2
  // but dst and src2 are the same (see constraint on instruction definition),
  // so we skip the first operand here (dst)
  unsigned int i = 0;
  if (instFormat == FARA::InstFormatALU)
    i = 1;

  for (; i < MI.getNumOperands(); i++) {
    const auto &operand = MI.getOperand(i);
    if (operand.isImm()) {
      int64_t imm = operand.getImm();
      support::endian::write<int64_t>(OS, imm, support::big);
    } else if (operand.isReg()) {
      unsigned reg = operand.getReg();
      support::endian::write<uint8_t>(OS, reg, support::little);
    }
  }

  ++MCNumEmitted; // Keep track of the # of mi's emitted.
}

#include "FARAGenMCCodeEmitter.inc"

MCCodeEmitter *llvm::createFARAMCCodeEmitter(const MCInstrInfo &MCII,
                                             MCContext &Ctx) {
  return new FARAMCCodeEmitter(Ctx, MCII);
}
