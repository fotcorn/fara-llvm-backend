//===-- FARAMCExpr.cpp - FARA specific MC expression classes --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the implementation of the assembly expression modifiers
// accepted by the FARA architecture (e.g. ":lo12:", ":gottprel_g1:", ...).
//
//===----------------------------------------------------------------------===//

#include "FARAMCExpr.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCObjectStreamer.h"
using namespace llvm;

#define DEBUG_TYPE "faramcexpr"

const FARAMCExpr *FARAMCExpr::create(const MCExpr *Expr, VariantKind Kind,
                                     MCContext &Ctx) {
  return new (Ctx) FARAMCExpr(Expr, Kind);
}

bool FARAMCExpr::evaluateAsRelocatableImpl(MCValue &Res,
                                           const MCAsmLayout *Layout,
                                           const MCFixup *Fixup) const {
  return getSubExpr()->evaluateAsRelocatable(Res, Layout, Fixup);
}

void FARAMCExpr::printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const {
  const MCExpr *Expr = getSubExpr();
  Expr->print(OS, MAI);
}

void FARAMCExpr::visitUsedExpr(MCStreamer &Streamer) const {
  Streamer.visitUsedExpr(*getSubExpr());
}

MCFragment *FARAMCExpr::findAssociatedFragment() const {
  return getSubExpr()->findAssociatedFragment();
}

void FARAMCExpr::fixELFSymbolsInTLSFixups(MCAssembler &) const {}
