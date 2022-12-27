//===-- FARAISelLowering.cpp - FARA DAG Lowering Implementation  ----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that FARA uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#include "FARAISelLowering.h"
#include "FARASubtarget.h"

using namespace llvm;

#define DEBUG_TYPE "fara-lower"

FARATargetLowering::FARATargetLowering(const TargetMachine &TM,
                                       const FARASubtarget &STI)
    : TargetLowering(TM), Subtarget(STI) {}
