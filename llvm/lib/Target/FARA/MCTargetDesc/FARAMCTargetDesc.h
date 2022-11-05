//===-- FARAMCTargetDesc.h - FARA Target Descriptions ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides FARA specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_FARA_MCTARGETDESC_FARAMCTARGETDESC_H
#define LLVM_LIB_TARGET_FARA_MCTARGETDESC_FARAMCTARGETDESC_H

#include "llvm/Config/config.h"
#include "llvm/MC/MCTargetOptions.h"
#include "llvm/Support/DataTypes.h"
#include <memory>

namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectTargetWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class Target;

// implemented in FARAAsmBackend.cpp
MCAsmBackend *createFARAAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                   const MCRegisterInfo &MRI,
                                   const MCTargetOptions &Options);

// implemented in FARAELFObjectWriter.cpp
std::unique_ptr<MCObjectTargetWriter> createFARAELFObjectWriter(uint8_t OSABI);
}

// Defines symbolic names for FARA registers.
#define GET_REGINFO_ENUM
#include "FARAGenRegisterInfo.inc"

// Defines symbolic names for FARA instructions.
#define GET_INSTRINFO_ENUM
#define GET_INSTRINFO_MC_HELPER_DECLS
#include "FARAGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "FARAGenSubtargetInfo.inc"

#endif
