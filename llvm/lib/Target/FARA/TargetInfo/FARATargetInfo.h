//===-- FARATargetInfo.h - FARA Target Implementation ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_FARA_TARGETINFO_FARATARGETINFO_H
#define LLVM_LIB_TARGET_FARA_TARGETINFO_FARATARGETINFO_H

namespace llvm {

class Target;

Target &getTheFARATarget();

} // namespace llvm

#endif // LLVM_LIB_TARGET_FARA_TARGETINFO_FARATARGETINFO_H
