//===-- FARA.h - Top-level interface for FARA representation ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// FARA back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_FARA_FARA_H
#define LLVM_LIB_TARGET_FARA_FARA_H

#include "MCTargetDesc/FARAMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class FunctionPass;
class FARATargetMachine;
class MachineInstr;
class MCInst;
class AsmPrinter;

FunctionPass *createFARAISelDag(FARATargetMachine &TM);
void lowerFARAMachineInstrToMCInst(const MachineInstr *MI, MCInst &OutMI,
                                   AsmPrinter &AP);
} // end namespace llvm
#endif
