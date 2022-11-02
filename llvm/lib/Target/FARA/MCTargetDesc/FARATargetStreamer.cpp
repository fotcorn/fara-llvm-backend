//===-- FARATargetStreamer.cpp - FARA Target Streamer Methods -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides FARA specific target streamer methods.
//
//===----------------------------------------------------------------------===//

#include "FARATargetStreamer.h"
#include "FARAMCTargetDesc.h"
#include "llvm/Support/FormattedStream.h"

using namespace llvm;

FARATargetStreamer::FARATargetStreamer(MCStreamer &S) : MCTargetStreamer(S) {}

// This part is for ascii assembly output
FARATargetAsmStreamer::FARATargetAsmStreamer(MCStreamer &S,
                                               formatted_raw_ostream &OS)
    : FARATargetStreamer(S) {}
