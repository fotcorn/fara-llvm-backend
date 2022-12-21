include "FARAInstrFormats.td"

// arithmetic
def ADD8S_rr : ALU_rr<0x1, 0x1, "add8s $src1, $src2", [(set i64:$dst, (add i64:$src1, i64:$src2))]>;
def ADD8S_ir : ALU_ir<0x1, 0x1, "add8s $src1, $src2", [(set i64:$dst, (add i64:$src1, i64:$src2))]>;
def ADDS : MnemonicAlias<"adds", "add8s">;
def ADD : MnemonicAlias<"add", "add8s">;

def SUB8S_rr : ALU_rr<0x1, 0x2, "sub8s $src1, $src2", [(set i64:$dst, (sub i64:$src1, i64:$src2))]>;
def SUB8S_ir : ALU_ir<0x1, 0x2, "sub8s $src1, $src2", [(set i64:$dst, (sub i64:$src1, i64:$src2))]>;
def SUBS : MnemonicAlias<"subs", "sub8s">;
def SUB : MnemonicAlias<"sub", "sub8s">;

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
}

def MOV : MnemonicAlias<"mov", "mov8">;

def STR8 : FARAInst<0x0, 0x3, 0x4, // todo add isStore flag
  (outs i64imm_op:$dst),
  (ins IntRegs:$src),
  "str8 $src, $dst",
  [(set i64:$dst, i64:$src)], // todo: fix Pattern
  InstFormatOneOut>; // todo: implement encoding
def STR : MnemonicAlias<"str", "str8">;

def LD8 : FARAInst<0x0, 0x2, 0x4, // todo add isLoad flag
  (outs i64imm_op:$dst),
  (ins IntRegs:$src),
  "ld8 $src, $dst",
  [(set i64:$dst, i64:$src)], // todo: fix Pattern
  InstFormatOneOut>; // todo: implement encoding
def LD : MnemonicAlias<"ld", "ld8">;

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

// todo: we don't support size 0 yet in the emulator, writing as 8 for now
def HALT : FARAInst<0x8, 0x1, 0x4,
  (outs),
  (ins),
  "halt",
  [],
  InstFormatNoOut>
{
  let hasSideEffects = 1;
}
