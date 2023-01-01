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

def PC : FARAReg<0x01, "pc">, DwarfRegNum<[1]>;
def SP : FARAReg<0x02, "sp">, DwarfRegNum<[2]>;
def FP : FARAReg<0x03, "fp">, DwarfRegNum<[3]>;

def I0 : FARAReg<0x4, "i0">, DwarfRegNum<[4]>;
def I1 : FARAReg<0x5, "i1">, DwarfRegNum<[5]>;
def I2 : FARAReg<0x6, "i2">, DwarfRegNum<[6]>;
def I3 : FARAReg<0x7, "i3">, DwarfRegNum<[7]>;
def I4 : FARAReg<0x8, "i4">, DwarfRegNum<[8]>;
def I5 : FARAReg<0x9, "i5">, DwarfRegNum<[9]>;
def I6 : FARAReg<0xa, "i6">, DwarfRegNum<[10]>;
def I7 : FARAReg<0xb, "i7">, DwarfRegNum<[11]>;
def I8 : FARAReg<0xc, "i8">, DwarfRegNum<[12]>;
def I9 : FARAReg<0xd, "i9">, DwarfRegNum<[13]>;
def I10 : FARAReg<0xe, "i10">, DwarfRegNum<[14]>;
def I11 : FARAReg<0xf, "i11">, DwarfRegNum<[15]>;
def I12 : FARAReg<0x10, "i12">, DwarfRegNum<[16]>;
def I13 : FARAReg<0x11, "i13">, DwarfRegNum<[17]>;
def I14 : FARAReg<0x12, "i14">, DwarfRegNum<[18]>;
def I15 : FARAReg<0x13, "i15">, DwarfRegNum<[19]>;
def I16 : FARAReg<0x14, "i16">, DwarfRegNum<[20]>;
def I17 : FARAReg<0x15, "i17">, DwarfRegNum<[21]>;
def I18 : FARAReg<0x16, "i18">, DwarfRegNum<[22]>;
def I19 : FARAReg<0x17, "i19">, DwarfRegNum<[23]>;
def I20 : FARAReg<0x18, "i20">, DwarfRegNum<[24]>;
def I21 : FARAReg<0x19, "i21">, DwarfRegNum<[25]>;
def I22 : FARAReg<0x1a, "i22">, DwarfRegNum<[26]>;
def I23 : FARAReg<0x1b, "i23">, DwarfRegNum<[27]>;

def IntRegs : RegisterClass<"FARA", [i64], 64,
  (add I0, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15, I16, I17, I18, I19, I20, I21, I22, I23)>;

def AllRegs : RegisterClass<"FARA", [i64], 64,
  (add I0, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15, I16, I17, I18, I19, I20, I21, I22, I23, PC, SP, FP)>;
