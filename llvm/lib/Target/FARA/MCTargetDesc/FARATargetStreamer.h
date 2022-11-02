//===-- FARATargetStreamer.h - FARA Target Streamer ----------*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_FARA_MCTARGETDESC_FARATARGETSTREAMER_H
#define LLVM_LIB_TARGET_FARA_MCTARGETDESC_FARATARGETSTREAMER_H

#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"

namespace llvm {

class formatted_raw_ostream;

class FARATargetStreamer : public MCTargetStreamer {

public:
  FARATargetStreamer(MCStreamer &S);
};

// This part is for ascii assembly output
class FARATargetAsmStreamer : public FARATargetStreamer {
public:
  FARATargetAsmStreamer(MCStreamer &S, formatted_raw_ostream &OS);
};

} // namespace llvm

#endif
