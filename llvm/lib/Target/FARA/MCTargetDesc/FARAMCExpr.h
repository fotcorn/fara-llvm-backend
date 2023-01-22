//===-- FARAMCExpr.h - FARA specific MC expression classes ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes FARA-specific MCExprs.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_FARA_MCTARGETDESC_FARAMCEXPR_H
#define LLVM_LIB_TARGET_FARA_MCTARGETDESC_FARAMCEXPR_H

#include "llvm/MC/MCExpr.h"

namespace llvm {

class FARAMCExpr : public MCTargetExpr {
public:
  enum VariantKind {
    VK_FARA_None,
    VK_FARA_CALL,
  };

private:
  const MCExpr *Expr;
  const VariantKind Kind;

  explicit FARAMCExpr(const MCExpr *Expr, VariantKind Kind)
      : Expr(Expr), Kind(Kind) {}

public:
  static const FARAMCExpr *create(const MCExpr *Expr, VariantKind Kind,
                                  MCContext &Ctx);

  void printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const override;
  bool evaluateAsRelocatableImpl(MCValue &Res, const MCAsmLayout *Layout,
                                 const MCFixup *Fixup) const override;
  void visitUsedExpr(MCStreamer &Streamer) const override;
  MCFragment *findAssociatedFragment() const override;
  void fixELFSymbolsInTLSFixups(MCAssembler &) const override;

  VariantKind getKind() const { return Kind; }
  const MCExpr *getSubExpr() const { return Expr; }
};

} // end namespace llvm.

#endif
