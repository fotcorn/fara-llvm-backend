//===-- FARAELFObjectWriter.cpp - FARA ELF Writer
//-----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "FARAMCTargetDesc.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {
class FARAELFObjectWriter : public MCELFObjectTargetWriter {
public:
  FARAELFObjectWriter(uint8_t OSABI)
      : MCELFObjectTargetWriter(/* Is64Bit */ true, OSABI, ELF::EM_FARA,
                                /* HasRelocationAddend */ true) {}

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;

  bool needsRelocateWithSymbol(const MCSymbol &Sym, unsigned Type) const override {
    return true;
  }
};
} // namespace

unsigned FARAELFObjectWriter::getRelocType(MCContext &Ctx,
                                           const MCValue &Target,
                                           const MCFixup &Fixup,
                                           bool IsPCRel) const {
  return 0;
}

std::unique_ptr<MCObjectTargetWriter>
llvm::createFARAELFObjectWriter(uint8_t OSABI) {
  return std::make_unique<FARAELFObjectWriter>(OSABI);
}
