include "FARAInstrFormats.td"

def ADD8SReg : FARARegRegInst<0x0101, "add8s $src1, $src2", [(set i64:$dst, (add i64:$src1, i64:$src2))]>;
def ADD8SImm : FARAImmRegInst<0x0101, "add8s $src1, $src2", [(set i64:$dst, (add i64:$src1, i64:$src2))]>;
def ADDS : MnemonicAlias<"adds", "add8s">;
def ADD : MnemonicAlias<"add", "add8s">;

def SUB8SReg : FARARegRegInst<0x0102, "sub8s $src1, $src2", [(set i64:$dst, (sub i64:$src1, i64:$src2))]>;
def SUB8SImm : FARAImmRegInst<0x0102, "sub8s $src1, $src2", [(set i64:$dst, (sub i64:$src1, i64:$src2))]>;
def SUBS : MnemonicAlias<"subs", "sub8s">;
def SUB : MnemonicAlias<"sub", "sub8s">;

def MOV8Reg : FARAInst<0x0001, 8,
  (outs IntRegs:$dst),
  (ins IntRegs:$src),
  "mov8 $src, $dst",
  [(set i64:$dst, i64:$src)]>;

def MOV8Imm : FARAInst<0x0001, 8,
  (outs IntRegs:$dst),
  (ins i64imm:$src),
  "mov8 $src, $dst",
  [(set i64:$dst, i64:$src)]>;

def MOV : MnemonicAlias<"mov", "mov8">;

def STR8 : FARAInst<0x0003, 8,
  (outs i64imm:$dst),
  (ins IntRegs:$src),
  "str8 $src, $dst",
  [(set i64:$dst, i64:$src)]>;
def STR : MnemonicAlias<"str", "str8">;

def LD8 : FARAInst<0x0002, 8,
  (outs i64imm:$dst),
  (ins IntRegs:$src),
  "ld8 $src, $dst",
  [(set i64:$dst, i64:$src)]>;
def LD : MnemonicAlias<"ld", "ld8">;

def OUT1Imm : FARAInst<0x0602, 1,
  (outs),
  (ins i8imm:$descriptor, i8imm:$character),
  "out1 $descriptor, $character",
  []>
{
  let hasSideEffects = 1;
}

def OUT1Reg : FARAInst<0x0602, 1,
  (outs),
  (ins i8imm:$descriptor, IntRegs:$character),
  "out1 $descriptor, $character",
  []>
{
  let hasSideEffects = 1;
}
def OUT : MnemonicAlias<"out", "out1">;

def HALT : FARAInst<0x0801, 0,
  (outs),
  (ins),
  "halt",
  []>
{
  let hasSideEffects = 1;
}
