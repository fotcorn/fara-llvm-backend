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

def i64imm_op : Operand<i64> {
  let OperandNamespace = "FARA";
  let OperandType = "OPERAND_I64IMM";
}

// These definitions must be kept in-sync with FARABaseInfo.h.
class InstFormat<bits<4> val> {
  bits<4> Value = val;
}
def InstFormatDefault : InstFormat<0>;
def InstFormatALU : InstFormat<1>; // src2 operand is also dst

class FARAInst<bits<8> optype, bits<8> opcode, bits<4> size, dag outs, dag ins, string asmstr, list<dag> pattern, InstFormat format=InstFormatDefault>
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
  : FARAInst<optype, opcode, 0x4, (outs IntRegs:$dst), (ins IntRegs:$src1, IntRegs:$src2), asmstr, pattern, InstFormatALU> {
  let Inst{19-16} = 0x1; // register
  let Inst{31-28} = 0x1; // register
  let Constraints = "$src2 = $dst";
}

class ALU_ir<bits<8> optype, bits<8> opcode, string asmstr, list<dag> pattern>
  : FARAInst<optype, opcode, 0x4, (outs IntRegs:$dst), (ins i64imm_op:$src1, IntRegs:$src2), asmstr, pattern, InstFormatALU> {
  let Inst{19-16} = 0x5; // 8 byte immediate
  let Inst{31-28} = 0x1; // register
  let Constraints = "$src2 = $dst";
}
