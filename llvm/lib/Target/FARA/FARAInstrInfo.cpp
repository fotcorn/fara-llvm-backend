//===-- FARAInstrInfo.cpp - FARA Instruction Information ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the FARA implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "FARAInstrInfo.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "FARAGenInstrInfo.inc"

FARAInstrInfo::FARAInstrInfo(FARASubtarget &ST)
    : FARAGenInstrInfo() {}
