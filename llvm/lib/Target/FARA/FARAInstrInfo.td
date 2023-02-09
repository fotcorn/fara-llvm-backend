include "FARAInstrFormats.td"

def retflag : SDNode<"FARAISD::RET_FLAG", SDTNone,
                     [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

//  These are target-independent nodes, but have target-specific formats.
def SDT_FARACallSeqStart : SDCallSeqStart<[ SDTCisVT<0, i64>,
                                          SDTCisVT<1, i64> ]>;
def SDT_FARACallSeqEnd   : SDCallSeqEnd<[ SDTCisVT<0, i64>,
                                        SDTCisVT<1, i64> ]>;

def callseq_start : SDNode<"ISD::CALLSEQ_START", SDT_FARACallSeqStart,
                           [SDNPHasChain, SDNPOutGlue]>;
def callseq_end   : SDNode<"ISD::CALLSEQ_END",   SDT_FARACallSeqEnd,
                           [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

def SDT_FARACall    : SDTypeProfile<0, -1, [SDTCisVT<0, i32>]>;
def call          : SDNode<"FARAISD::CALL", SDT_FARACall,
                           [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
                            SDNPVariadic]>;

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

def SL8S_rr : ALU_rr<0x2, 0x5, "sl8s $src1, $src2", [(set i64:$dst, (shl i64:$src1, i64:$src2))]> {
  let Constraints = "$src1 = $dst";
}
def SL8S_ir : ALU_ir<0x2, 0x5, "sl8s $src1, $src2", [(set i64:$dst, (shl i64:$src1, i64:$src2))]> {
  let Constraints = "$src1 = $dst";
}
def SLS : MnemonicAlias<"sls", "sl8s">;
def SL8 : MnemonicAlias<"sl8", "sl8s">;
def SL : MnemonicAlias<"sl", "sl8s">;

def SR8S_rr : ALU_rr<0x2, 0x6, "sr8s $src1, $src2", [(set i64:$dst, (srl i64:$src1, i64:$src2))]> {
  let Constraints = "$src1 = $dst";
}
def SR8S_ir : ALU_ir<0x2, 0x6, "sr8s $src1, $src2", [(set i64:$dst, (srl i64:$src1, i64:$src2))]> {
  let Constraints = "$src1 = $dst";
}
def SRS : MnemonicAlias<"srs", "sr8s">;
def SR8 : MnemonicAlias<"sr8", "sr8s">;
def SR : MnemonicAlias<"sr", "sr8s">;

// mov/memory
def MOV8_rr : FARAInst<0x0, 0x1, 0x4,
  (outs IntRegs:$dst),
  (ins IntRegs:$src),
  "mov8 $src, $dst",
  [],
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
  [],
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

// indexed store
class Store_rrr<string sizeString, bits<4> size, PatFrag op> : FARAInst<0x0, 0x5, size,
  (outs),
  (ins IntRegs:$data, (MEMrr $r1, $r2):$address),
  !strconcat("strx", sizeString, " $data, $address"),
  [(op i64:$data, ADDRrr:$address)],
  InstFormatNoOut>
{
  let Inst{19-16} = 0x1; // register
  let Inst{31-28} = 0x1; // register
  let Inst{27-24} = 0x1; // register

  let mayStore = true;
}
class Store_rri<string sizeString, bits<4> size, PatFrag op> : FARAInst<0x0, 0x5, size,
  (outs),
  (ins IntRegs:$data, (MEMri $r, $imm):$address),
  !strconcat("strx", sizeString, " $data, $address"),
  [(op i64:$data, ADDRri:$address)],
  InstFormatNoOut>
{
  let Inst{19-16} = 0x1; // register
  let Inst{31-28} = 0x1; // register
  let Inst{27-24} = 0x5; // 8 byte immediate
  let mayStore = true;
}
multiclass Store<string sizeString, bits<4> size, PatFrag op> {
  def _rrr : Store_rri<sizeString, size, op>;
  def _rri : Store_rri<sizeString, size, op>;
}
defm STRX8 : Store<"8", 0x4, store>;
defm STRX4 : Store<"4", 0x3, truncstorei32>;
defm STRX2 : Store<"2", 0x2, truncstorei16>;
defm STRX1 : Store<"1", 0x1, truncstorei8>;
def STRX : MnemonicAlias<"strx", "strx8">;

// indexed load
class Load_rrr<string sizeString, bits<4> size, PatFrag op> : FARAInst<0x0, 0x4, size,
  (outs IntRegs:$target),
  (ins (MEMrr $r1, $r2):$address),
  !strconcat("ldx", sizeString, " $address, $target"),
  [(set i64:$target, (op ADDRrr:$address))],
  InstFormatOneOut>
{
  let Inst{19-16} = 0x1; // register
  let Inst{31-28} = 0x1; // register
  let Inst{27-24} = 0x1; // register
  let mayLoad = true;
}
class Load_rir<string sizeString, bits<4> size, PatFrag op> : FARAInst<0x0, 0x4, size,
  (outs IntRegs:$target),
  (ins (MEMri $r, $imm):$address),
  !strconcat("ldx", sizeString, " $address, $target"),
  [(set i64:$target, (op ADDRri:$address))],
  InstFormatOneOut>
{
  let Inst{19-16} = 0x1; // register
  let Inst{31-28} = 0x5; // 8 byte immediate
  let Inst{27-24} = 0x1; // register
  let mayLoad = true;
}
multiclass Load<string sizeString, bits<4> size, PatFrag op> {
  def _rrr : Load_rrr<sizeString, size, op>;
  def _rir : Load_rir<sizeString, size, op>;
}
defm LDX8 : Load<"8", 0x4, load>;
defm LDX4 : Load<"4", 0x3, extloadi32>;
defm LDX2 : Load<"2", 0x2, extloadi16>;
defm LDX1 : Load<"1", 0x1, extloadi8>;
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
  (ins calltarget:$target),
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
  let isBranch = 1;
  let isTerminator = 1;
}

defm JE : CondJump<0x3, 0x2, "je", seteq>;
def : MnemonicAlias<"je", "je8">;

defm JNE : CondJump<0x3, 0x3, "jne", setne>;
def : MnemonicAlias<"jne", "jne8">;

// lower
defm JLS : CondJump<0x4, 0x1, "jls", setlt>;
def : MnemonicAlias<"jls", "jls8">;
def : MnemonicAlias<"jl", "jls8">;
defm JLU : CondJump<0x4, 0x2, "jlu", setult>;
def : MnemonicAlias<"jlu", "jlu8">;

defm JLES : CondJump<0x4, 0x3, "jles", setle>;
def : MnemonicAlias<"jles", "jles8">;
def : MnemonicAlias<"jle", "jles8">;
defm JLEU : CondJump<0x4, 0x4, "jleu", setule>;
def : MnemonicAlias<"jleu", "jleu8">;

// greater
defm JGS : CondJump<0x4, 0x5, "jgs", setgt>;
def : MnemonicAlias<"jgs", "jgs8">;
def : MnemonicAlias<"jg", "jgs8">;
defm JGU : CondJump<0x4, 0x6, "jgu", setugt>;
def : MnemonicAlias<"jgu", "jgu8">;

defm JGES : CondJump<0x4, 0x7, "jges", setge>;
def : MnemonicAlias<"jges", "jges8">;
def : MnemonicAlias<"jge", "jges8">;
defm JGEU : CondJump<0x4, 0x8, "jgeu", setuge>;
def : MnemonicAlias<"jgeu", "jgeu8">;

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

// Immediates
def : Pat<(i64 i64imm_op:$imm), (MOV8_ir i64imm_op:$imm)>;

// Calls
def : Pat<(call tglobaladdr:$dst),
          (CALL tglobaladdr:$dst)>;
def : Pat<(call texternalsym:$dst),
          (CALL texternalsym:$dst)>;

let Defs = [SP], Uses = [SP] in {
def ADJCALLSTACKDOWN : Pseudo<(outs), (ins i64imm_op:$amt1, i64imm_op:$amt2),
                              [(callseq_start timm:$amt1, timm:$amt2)]>;
def ADJCALLSTACKUP   : Pseudo<(outs), (ins i64imm_op:$amt1, i64imm_op:$amt2),
                              [(callseq_end timm:$amt1, timm:$amt2)]>;
}
