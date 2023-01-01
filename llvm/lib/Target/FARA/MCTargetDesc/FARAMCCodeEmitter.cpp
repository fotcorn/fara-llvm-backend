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
  MCContext &Ctx;

public:
  FARAMCCodeEmitter(MCContext &ctx, MCInstrInfo const &MCII) : MCII(MCII), Ctx(ctx) {}
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

private:
  void encodeOperand(const unsigned int operandIndex, const MCInst &MI,
                     const MCInstrDesc &Desc, SmallVectorImpl<MCFixup> &Fixups,
                     uint64_t InstrStartPos, raw_ostream &OS) const;
};
} // end anonymous namespace

void FARAMCCodeEmitter::encodeOperand(const unsigned int operandIndex,
                                      const MCInst &MI, const MCInstrDesc &Desc,
                                      SmallVectorImpl<MCFixup> &Fixups,
                                      uint64_t InstrStartPos,
                                      raw_ostream &OS) const {
  const MCOperand &operand = MI.getOperand(operandIndex);
  const MCOperandInfo &operandInfo = Desc.OpInfo[operandIndex];
  if (operand.isImm()) {
    int64_t imm = operand.getImm();
    switch (operandInfo.OperandType) {
    case FARA::OPERAND_I8IMM:
      support::endian::write<int8_t>(OS, imm, support::big);
      break;
    case FARA::OPERAND_I16IMM:
      support::endian::write<int16_t>(OS, imm, support::big);
      break;
    case FARA::OPERAND_I32IMM:
      support::endian::write<int32_t>(OS, imm, support::big);
      break;
    case FARA::OPERAND_I64IMM:
      support::endian::write<int64_t>(OS, imm, support::big);
      break;
    default:
      llvm_unreachable("invalid operand type");
    }
  } else if (operand.isReg()) {
    uint16_t reg = Ctx.getRegisterInfo()->getEncodingValue(operand.getReg());
    support::endian::write<uint8_t>(OS, reg, support::big);
  } else if (operand.isExpr()) {
    const MCExpr *expr = operand.getExpr();
    Fixups.push_back(MCFixup::create(OS.tell() - InstrStartPos, expr,
                                     MCFixupKind::FK_PCRel_8));
    support::endian::write<int64_t>(OS, 0, support::big);
  }
}

void FARAMCCodeEmitter::encodeInstruction(const MCInst &MI, raw_ostream &OS,
                                          SmallVectorImpl<MCFixup> &Fixups,
                                          const MCSubtargetInfo &STI) const {
  uint64_t InstrStartPos = OS.tell();
  uint32_t Bits = getBinaryCodeForInstr(MI, Fixups, STI);
  support::endian::write<uint32_t>(OS, Bits, support::little);

  const MCInstrDesc &Desc = MCII.get(MI.getOpcode());
  unsigned int instFormat = FARA::getFormat(Desc.TSFlags);

  // the list of operands contains the out operands first, then the ins operands
  // in machine code, we serialize ins first, then outs, so we have to invert
  // the order depending on the instruction format.
  // * InstFormatNoOut: We don't have any outs, so just start at index 0 with
  // the first ins
  // * InstFormatOneOut: One outs, so start with index 1, and output out after
  // the iteration
  // * InstFormatOneOutIgnore: Also one outs, start at index 1, but then don't
  // output outs
  unsigned int I = instFormat == FARA::InstFormatNoOut ? 0 : 1;
  // iterate over ins operands
  for (; I < MI.getNumOperands(); I++) {
    encodeOperand(I, MI, Desc, Fixups, InstrStartPos, OS);
  }

  if (instFormat == FARA::InstFormatOneOut)
    encodeOperand(0, MI, Desc, Fixups, InstrStartPos, OS);

  ++MCNumEmitted; // Keep track of the # of mi's emitted.
}

#include "FARAGenMCCodeEmitter.inc"

MCCodeEmitter *llvm::createFARAMCCodeEmitter(const MCInstrInfo &MCII,
                                             MCContext &Ctx) {
  return new FARAMCCodeEmitter(Ctx, MCII);
}
