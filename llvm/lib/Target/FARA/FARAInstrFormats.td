/*
Inst{7-0} = optype
Inst{15-8} = opcode
Inst{19-16} = operand1
Inst{23-20} = size
Inst{27-24} = operand3
Inst{31-28} = operand2
*/

class FARAInst<bits<8> optype, bits<8> opcode, bits<4> size, dag outs, dag ins, string asmstr, list<dag> pattern>
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
}

class FARARegRegInstArithmetic<bits<8> optype, bits<8> opcode, string asmstr, list<dag> pattern>
  : FARAInst<optype, opcode, 0x4, (outs IntRegs:$dst), (ins IntRegs:$src1, IntRegs:$src2), asmstr, pattern> {
  let Inst{19-16} = 0x1; // register
  let Inst{31-28} = 0x1; // register
  let Constraints = "$src2 = $dst";
}

class FARAImmRegInstArithmetic<bits<8> optype, bits<8> opcode, string asmstr, list<dag> pattern>
  : FARAInst<optype, opcode, 0x4, (outs IntRegs:$dst), (ins i64imm:$src1, IntRegs:$src2), asmstr, pattern> {
  let Inst{19-16} = 0x5; // 8 byte immediate
  let Inst{31-28} = 0x1; // register
  let Constraints = "$src2 = $dst";
}
