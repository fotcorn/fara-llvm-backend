//===-- FARARegisterInfo.td - FARA Register defs ---------------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the FARA register file
//===----------------------------------------------------------------------===//

class FARAReg<bits<8> Enc, string n> : Register<n> {
    let HWEncoding{15-8} = 0;
    let HWEncoding{7-0} = Enc;
    let Namespace = "FARA";
}

def I0 : FARAReg<1, "i0">, DwarfRegNum<[1]>;
def I1 : FARAReg<2, "i1">, DwarfRegNum<[2]>;
def I2 : FARAReg<3, "i2">, DwarfRegNum<[3]>;
def I3 : FARAReg<4, "i3">, DwarfRegNum<[4]>;
def I4 : FARAReg<5, "i4">, DwarfRegNum<[5]>;
def I5 : FARAReg<6, "i5">, DwarfRegNum<[6]>;
def I6 : FARAReg<7, "i6">, DwarfRegNum<[7]>;
def I7 : FARAReg<8, "i7">, DwarfRegNum<[8]>;

def PC : FARAReg<9, "pc">, DwarfRegNum<[9]>;
def SP : FARAReg<10, "sp">, DwarfRegNum<[10]>;

def IntRegs : RegisterClass<"FARA", [i64], 64,
  (add I0, I1, I2, I3, I4, I5, I6, I7)>;

def AllRegs : RegisterClass<"FARA", [i64], 64,
  (add I0, I1, I2, I3, I4, I5, I6, I7, PC, SP)>;
