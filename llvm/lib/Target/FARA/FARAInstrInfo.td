include "FARAInstrFormats.td"


def ADD8SReg : FARARegRegInstArithmetic<0x1, 0x1, "add8s $src1, $src2", [(set i64:$dst, (add i64:$src1, i64:$src2))]>;
def ADD8SImm : FARAImmRegInstArithmetic<0x1, 0x1, "add8s $src1, $src2", [(set i64:$dst, (add i64:$src1, i64:$src2))]>;
def ADDS : MnemonicAlias<"adds", "add8s">;
def ADD : MnemonicAlias<"add", "add8s">;

def SUB8SReg : FARARegRegInstArithmetic<0x1, 0x2, "sub8s $src1, $src2", [(set i64:$dst, (sub i64:$src1, i64:$src2))]>;
def SUB8SImm : FARAImmRegInstArithmetic<0x1, 0x2, "sub8s $src1, $src2", [(set i64:$dst, (sub i64:$src1, i64:$src2))]>;
def SUBS : MnemonicAlias<"subs", "sub8s">;
def SUB : MnemonicAlias<"sub", "sub8s">;

def MOV8Reg : FARAInst<0x0, 0x1, 0x4,
  (outs IntRegs:$dst),
  (ins IntRegs:$src),
  "mov8 $src, $dst",
  [(set i64:$dst, i64:$src)]>
{
  let Inst{19-16} = 0x1; // register
  let Inst{31-28} = 0x1; // register
}

def MOV8Imm : FARAInst<0x0, 0x1, 0x4,
  (outs IntRegs:$dst),
  (ins i64imm:$src),
  "mov8 $src, $dst",
  [(set i64:$dst, i64:$src)]>
{
  let Inst{19-16} = 0x5; // 8 byte immediate
  let Inst{31-28} = 0x1; // register
}

def MOV : MnemonicAlias<"mov", "mov8">;

def STR8 : FARAInst<0x0, 0x3, 0x4,
  (outs i64imm:$dst),
  (ins IntRegs:$src),
  "str8 $src, $dst",
  [(set i64:$dst, i64:$src)]>;
def STR : MnemonicAlias<"str", "str8">;

def LD8 : FARAInst<0x0, 0x2, 0x4,
  (outs i64imm:$dst),
  (ins IntRegs:$src),
  "ld8 $src, $dst",
  [(set i64:$dst, i64:$src)]>;
def LD : MnemonicAlias<"ld", "ld8">;

// todo: OUT is only valid for 1 byte for each parameter
def OUT1Imm : FARAInst<0x6, 0x2, 0x4,
  (outs),
  (ins i8imm:$descriptor, i8imm:$character),
  "out1 $descriptor, $character",
  []>
{
  let hasSideEffects = 1;
}

def OUT1Reg : FARAInst<0x6, 0x2, 0x4,
  (outs),
  (ins i8imm:$descriptor, IntRegs:$character),
  "out1 $descriptor, $character",
  []>
{
  let hasSideEffects = 1;
}
def OUT : MnemonicAlias<"out", "out1">;

def HALT : FARAInst<0x8, 0x1, 0,
  (outs),
  (ins),
  "halt",
  []>
{
  let hasSideEffects = 1;
}
