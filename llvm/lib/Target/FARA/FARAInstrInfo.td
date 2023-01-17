include "FARAInstrFormats.td"

def retflag : SDNode<"FARAISD::RET_FLAG", SDTNone,
                     [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

// arithmetic
def ADD8S_rr : ALU_rr<0x1, 0x1, "add8s $src1, $src2", [(set i64:$dst, (add i64:$src1, i64:$src2))]>;
def ADD8S_ir : ALU_ir<0x1, 0x1, "add8s $src1, $src2", [(set i64:$dst, (add i64:$src1, i64:$src2))]>;
def ADDS : MnemonicAlias<"adds", "add8s">;
def ADD8 : MnemonicAlias<"add8", "add8s">;
def ADD : MnemonicAlias<"add", "add8s">;

def SUB8S_rr : ALU_rr<0x1, 0x2, "sub8s $src1, $src2", [(set i64:$dst, (sub i64:$src1, i64:$src2))]>;
def SUB8S_ir : ALU_ir<0x1, 0x2, "sub8s $src1, $src2", [(set i64:$dst, (sub i64:$src1, i64:$src2))]>;
def SUBS : MnemonicAlias<"subs", "sub8s">;
def SUB8 : MnemonicAlias<"sub8", "sub8s">;
def SUB : MnemonicAlias<"sub", "sub8s">;

def MUL8S_rr : ALU_rr<0x1, 0x3, "mul8s $src1, $src2", [(set i64:$dst, (mul i64:$src1, i64:$src2))]>;
def MUL8S_ir : ALU_ir<0x1, 0x3, "mul8s $src1, $src2", [(set i64:$dst, (mul i64:$src1, i64:$src2))]>;
def MULS : MnemonicAlias<"muls", "mul8s">;
def MUL8 : MnemonicAlias<"mul8", "mul8s">;
def MUL : MnemonicAlias<"mul", "mul8s">;

def DIV8S_rr : ALU_rr<0x1, 0x4, "div8s $src1, $src2", [(set i64:$dst, (sdiv i64:$src1, i64:$src2))]>;
def DIV8S_ir : ALU_ir<0x1, 0x4, "div8s $src1, $src2", [(set i64:$dst, (sdiv i64:$src1, i64:$src2))]>;
def DIVS : MnemonicAlias<"divs", "div8s">;
def DIV8 : MnemonicAlias<"div8", "div8s">;
def DIV : MnemonicAlias<"div", "div8s">;

def MOD8S_rr : ALU_rr<0x1, 0x5, "mod8s $src1, $src2", [(set i64:$dst, (srem i64:$src1, i64:$src2))]>;
def MOD8S_ir : ALU_ir<0x1, 0x5, "mod8s $src1, $src2", [(set i64:$dst, (srem i64:$src1, i64:$src2))]>;
def MODS : MnemonicAlias<"mods", "mod8s">;
def MOD8 : MnemonicAlias<"mod8", "mod8s">;
def MOD : MnemonicAlias<"mod", "mod8s">;

// mov/memory
def MOV8_rr : FARAInst<0x0, 0x1, 0x4,
  (outs IntRegs:$dst),
  (ins IntRegs:$src),
  "mov8 $src, $dst",
  [(set i64:$dst, i64:$src)],
  InstFormatOneOut>
{
  let Inst{19-16} = 0x1; // register
  let Inst{31-28} = 0x1; // register
  let isMoveImm = true;
}

def MOV8_ir: FARAInst<0x0, 0x1, 0x4,
  (outs IntRegs:$dst),
  (ins i64imm_op:$src),
  "mov8 $src, $dst",
  [(set i64:$dst, i64:$src)],
  InstFormatOneOut>
{
  let Inst{19-16} = 0x5; // 8 byte immediate
  let Inst{31-28} = 0x1; // register
  let isMoveReg = true;
}
def MOV : MnemonicAlias<"mov", "mov8">;

def STR8_rr : FARAInst<0x0, 0x3, 0x4,
  (outs),
  (ins IntRegs:$data, IntRegs:$address),
  "str8 $data, $address",
  [(store i64:$address, i64:$data)],
  InstFormatNoOut>
{
  let Inst{19-16} = 0x1; // register
  let Inst{31-28} = 0x1; // register
  let mayStore = true;
}
def STR : MnemonicAlias<"str", "str8">;

def LD8_rr : FARAInst<0x0, 0x2, 0x4,
  (outs IntRegs:$target),
  (ins IntRegs:$address),
  "ld8 $address, $target",
  [(set i64:$target, (load i64:$address))],
  InstFormatOneOut>
{
  let Inst{19-16} = 0x1; // register
  let Inst{31-28} = 0x1; // register
  let mayLoad = true;
}
def LD : MnemonicAlias<"ld", "ld8">;

def STRX8_rrr : FARAInst<0x0, 0x5, 0x4,
  (outs),
  (ins IntRegs:$data, (MEMrr $r1, $r2):$address),
  "strx8 $data, $address",
  [(store i64:$data, ADDRrr:$address)],
  InstFormatNoOut>
{
  let Inst{19-16} = 0x1; // register
  let Inst{31-28} = 0x1; // register
  let Inst{27-24} = 0x1; // register

  let mayStore = true;
}
def STRX8_rri : FARAInst<0x0, 0x5, 0x4,
  (outs),
  (ins IntRegs:$data, (MEMri $r, $imm):$address),
  "strx8 $data, $address",
  [(store i64:$data, ADDRri:$address)],
  InstFormatNoOut>
{
  let Inst{19-16} = 0x1; // register
  let Inst{31-28} = 0x1; // register
  let Inst{27-24} = 0x5; // 8 byte immediate
  let mayStore = true;
}
def STRX : MnemonicAlias<"strx", "strx8">;

def LDX8_rrr : FARAInst<0x0, 0x4, 0x4,
  (outs IntRegs:$target),
  (ins (MEMrr $r1, $r2):$address),
  "ldx8 $address, $target",
  [(set i64:$target, (load ADDRrr:$address))],
  InstFormatOneOut>
{
  let Inst{19-16} = 0x1; // register
  let Inst{31-28} = 0x1; // register
  let Inst{27-24} = 0x1; // register
  let mayLoad = true;
}
def LDX8_rir : FARAInst<0x0, 0x4, 0x4,
  (outs IntRegs:$target),
  (ins (MEMri $r, $imm):$address),
  "ldx8 $address, $target",
  [(set i64:$target, (load ADDRri:$address))],
  InstFormatOneOut>
{
  let Inst{19-16} = 0x1; // register
  let Inst{31-28} = 0x5; // 8 byte immediate
  let Inst{27-24} = 0x1; // register
  let mayLoad = true;
}
def LDX : MnemonicAlias<"ldx", "ldx8">;

// stack operations
def PUSH8_r : FARAInst<0x5, 0x1, 0x4,
  (outs),
  (ins IntRegs:$data),
  "push8 $data",
  [],
  InstFormatNoOut>
{
  let Inst{19-16} = 0x1; // register
  let mayStore = true;
}
def PUSH8_i : FARAInst<0x5, 0x1, 0x4,
  (outs),
  (ins i64imm_op:$data),
  "push8 $data",
  [],
  InstFormatNoOut>
{
  let Inst{19-16} = 0x5; // 8 byte immediate
  let mayStore = true;
}
def PUSH : MnemonicAlias<"push", "push8">;

def POP8_r : FARAInst<0x5, 0x2, 0x4,
  (outs IntRegs:$dst),
  (ins),
  "pop8 $dst",
  [],
  InstFormatOneOut>
{
  let Inst{19-16} = 0x1; // register
  let mayLoad = true;
}
def POP : MnemonicAlias<"pop", "pop8">;

def RET : FARAInst<0x5, 0x4, 0x0,
  (outs),
  (ins),
  "ret",
  [(retflag)],
  InstFormatNoOut>
{
  let isReturn = 1;
  let isTerminator = 1;
}

def CALL : FARAInst<0x5, 0x3, 0x4,
  (outs),
  (ins brtarget:$target),
  "call $target",
  [],
  InstFormatNoOut>
{
  let Inst{19-16} = 0x5; // 8 byte branch target
  let isCall = 1;
}

// jumps/branch
def JMP : FARAInst<0x3, 0x1, 0x4,
  (outs),
  (ins brtarget:$target),
  "jmp $target",
  [(br bb:$target)],
  InstFormatNoOut>
{
  let Inst{19-16} = 0x5; // 8 byte branch target
}

defm JE8 : CondJump<0x3, 0x2, "je8 $op1, $op2, $target", [(brcond (i64 (setne i64:$op1, i64:$op2)), bb:$target)]>;
def JE : MnemonicAlias<"je", "je8">;
defm JNE8 : CondJump<0x3, 0x3, "jne8 $op1, $op2, $target", [(brcond (i64 (seteq i64:$op1, i64:$op2)), bb:$target)]>;
def JNE : MnemonicAlias<"jne", "jne8">;

// i/o
def OUT1_ii : FARAInst<0x6, 0x2, 0x1,
  (outs),
  (ins i8imm_op:$descriptor, i8imm_op:$character),
  "out1 $descriptor, $character",
  [],
  InstFormatNoOut>
{
  let hasSideEffects = 1;
  let Inst{19-16} = 0x2; // 1 byte immediate
  let Inst{31-28} = 0x2; // 1 byte immediate
}

def OUT1_ir : FARAInst<0x6, 0x2, 0x1,
  (outs),
  (ins i8imm_op:$descriptor, IntRegs:$character),
  "out1 $descriptor, $character",
  [],
  InstFormatNoOut>
{
  let hasSideEffects = 1;
  let Inst{19-16} = 0x2; // 1 byte immediate
  let Inst{31-28} = 0x1; // register
}
def OUT : MnemonicAlias<"out", "out1">;

def HALT : FARAInst<0x8, 0x1, 0x0,
  (outs),
  (ins),
  "halt",
  [],
  InstFormatNoOut>
{
  let hasSideEffects = 1;
}

def : Pat<(i64 i64imm_op:$imm), (MOV8_ir i64imm_op:$imm)>;
