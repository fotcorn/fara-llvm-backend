/*
Inst{7-0} = optype
Inst{15-8} = opcode
Inst{19-16} = operand1
Inst{23-20} = size
Inst{27-24} = operand3
Inst{31-28} = operand2
*/

def i8imm_op : Operand<i8> {
  let OperandNamespace = "FARA";
  let OperandType = "OPERAND_I8IMM";
}

def i16imm_op : Operand<i16> {
  let OperandNamespace = "FARA";
  let OperandType = "OPERAND_I16IMM";
}

def i32imm_op : Operand<i32> {
  let OperandNamespace = "FARA";
  let OperandType = "OPERAND_I32IMM";
}

def i64imm_op : Operand<i64>, ImmLeaf<i64, [{return isInt<64>(Imm);}]> {
  let OperandNamespace = "FARA";
  let OperandType = "OPERAND_I64IMM";
}

def brtarget : Operand<OtherVT> {
  let OperandType = "OPERAND_PCREL";
  let PrintMethod = "printBranchTarget";
}

def calltarget : Operand<i32>;

def MEMrr : Operand<iPTR> {
  let PrintMethod = "printMemOperand";
  let MIOperandInfo = (ops IntRegs:$r1, IntRegs:$r2);
}
def MEMri : Operand<iPTR> {
  let PrintMethod = "printMemOperand";
  let MIOperandInfo = (ops IntRegs:$r, i64imm_op);
}

def ADDRrr : ComplexPattern<iPTR, 2, "SelectADDRrr", [], []>;
def ADDRri : ComplexPattern<iPTR, 2, "SelectADDRri", [frameindex], []>;

// These definitions must be kept in-sync with FARABaseInfo.h.
class InstFormat<bits<4> val> {
  bits<4> Value = val;
}
def InstFormatNoOut : InstFormat<0>; // instruction has no out operands
def InstFormatOneOut : InstFormat<1>; // instruction has one out operand
// instruction has one out operand, but it is not serialized
// because output is the same as the last output. mostly used in ALU ops like add/sub
def InstFormatOneOutIgnore : InstFormat<2>;

class FARAInst<bits<8> optype, bits<8> opcode, bits<4> size, dag outs, dag ins, string asmstr, list<dag> pattern, InstFormat format>
  : Instruction {
  field bits<32> Inst;
  let Inst{7-0} = optype;
  let Inst{15-8} = opcode;
  let Inst{23-20} = size;

  let Namespace = "FARA";
  let AsmString = asmstr;
  let OutOperandList = outs;
  let InOperandList = ins;
  let Pattern = pattern;

  let TSFlags{3-0} = format.Value;
}

class ALU_rr<bits<8> optype, bits<8> opcode, string asmstr, list<dag> pattern>
  : FARAInst<optype, opcode, 0x4, (outs IntRegs:$dst), (ins IntRegs:$src1, IntRegs:$src2), asmstr, pattern, InstFormatOneOutIgnore> {
  let Inst{19-16} = 0x1; // register
  let Inst{31-28} = 0x1; // register
  let Constraints = "$src2 = $dst";
}

class ALU_ir<bits<8> optype, bits<8> opcode, string asmstr, list<dag> pattern>
  : FARAInst<optype, opcode, 0x4, (outs IntRegs:$dst), (ins i64imm_op:$src1, IntRegs:$src2), asmstr, pattern, InstFormatOneOutIgnore> {
  let Inst{19-16} = 0x5; // 8 byte immediate
  let Inst{31-28} = 0x1; // register
  let Constraints = "$src2 = $dst";
}

class CondJump_rr<bits<8> optype, bits<8> opcode, bits<4> size, string mnemonic, PatFrag compare> : FARAInst<
    optype, opcode, size, (outs), (ins IntRegs:$op1, IntRegs:$op2, brtarget:$target),
    !strconcat(mnemonic, " $op1, $op2, $target"), [(brcond (compare IntRegs:$op1, IntRegs:$op2), bb:$target)], InstFormatNoOut> {
  let Inst{19-16} = 0x1; // register
  let Inst{31-28} = 0x1; // register
  let Inst{27-24} = 0x5; // 8 byte branch target
  let isBranch = 1;
  let isTerminator = 1;
}

class CondJump_ir<bits<8> optype, bits<8> opcode, bits<4> size, string mnemonic, PatFrag compare> : FARAInst<
    optype, opcode, size, (outs), (ins i64imm_op:$op1, IntRegs:$op2, brtarget:$target),
    !strconcat(mnemonic, " $op1, $op2, $target"), [(brcond (compare i64imm_op:$op1, IntRegs:$op2), bb:$target)], InstFormatNoOut> {
  let Inst{19-16} = 0x5; // 8 byte immediate
  let Inst{31-28} = 0x1; // register
  let Inst{27-24} = 0x5; // 8 byte branch target
  let isBranch = 1;
  let isTerminator = 1;
}

class CondJump_ri<bits<8> optype, bits<8> opcode, bits<4> size, string mnemonic, PatFrag compare> : FARAInst<
    optype, opcode, size, (outs), (ins IntRegs:$op1, i64imm_op:$op2, brtarget:$target),
    !strconcat(mnemonic, " $op1, $op2, $target"), [(brcond (compare IntRegs:$op1, i64imm_op:$op2), bb:$target)], InstFormatNoOut> {
  let Inst{19-16} = 0x1; // register
  let Inst{31-28} = 0x5; // 8 byte immediate
  let Inst{27-24} = 0x5; // 8 byte branch target
  let isBranch = 1;
  let isTerminator = 1;
}

multiclass CondJump<bits<8> optype, bits<8> opcode, string mnemonic, PatFrag compare> {
  def 8_rr : CondJump_rr<optype, opcode, 0x4, !strconcat(mnemonic, "8"), compare>;
  def 4_rr : CondJump_rr<optype, opcode, 0x3, !strconcat(mnemonic, "4"), compare>;
  def 2_rr : CondJump_rr<optype, opcode, 0x2, !strconcat(mnemonic, "2"), compare>;
  def 1_rr : CondJump_rr<optype, opcode, 0x1, !strconcat(mnemonic, "1"), compare>;

  def 8_ri : CondJump_ir<optype, opcode, 0x4, !strconcat(mnemonic, "8"), compare>;
  def 4_ri : CondJump_ir<optype, opcode, 0x3, !strconcat(mnemonic, "4"), compare>;
  def 2_ri : CondJump_ir<optype, opcode, 0x2, !strconcat(mnemonic, "2"), compare>;
  def 1_ri : CondJump_ir<optype, opcode, 0x1, !strconcat(mnemonic, "1"), compare>;

  def 8_ir : CondJump_rr<optype, opcode, 0x4, !strconcat(mnemonic, "8"), compare>;
  def 4_ir : CondJump_rr<optype, opcode, 0x3, !strconcat(mnemonic, "4"), compare>;
  def 2_ir : CondJump_rr<optype, opcode, 0x2, !strconcat(mnemonic, "2"), compare>;
  def 1_ir : CondJump_rr<optype, opcode, 0x1, !strconcat(mnemonic, "1"), compare>;
}

class Pseudo<dag outs, dag ins, list<dag> pattern>
   : FARAInst<0, 0, 0, outs, ins, "", pattern, InstFormatNoOut> {
  let isCodeGenOnly = 1;
  let isPseudo = 1;
}
