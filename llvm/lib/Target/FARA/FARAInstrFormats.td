class FARAInst<bits<12> opcode, bits<4> size, dag outs, dag ins, string asmstr, list<dag> pattern>
  : Instruction {
  field bits<32> Inst;
  let Inst{0-11} = opcode;
  let Inst{12-15} = size;

  let Namespace = "FARA";
  let AsmString = asmstr;
  let OutOperandList = outs;
  let InOperandList = ins;
  let Pattern = pattern;
}

class FARARegRegInst<bits<12> opcode, string asmstr, list<dag> pattern>
  : FARAInst<opcode, 8, (outs IntRegs:$dst), (ins IntRegs:$src1, IntRegs:$src2), asmstr, pattern> {
  let Inst{16-19} = 0x1; // register
  let Inst{20-23} = 0x1; // register
  let Constraints = "$src2 = $dst";
}

class FARAImmRegInst<bits<12> opcode, string asmstr, list<dag> pattern>
  : FARAInst<opcode, 8, (outs IntRegs:$dst), (ins i64imm:$src1, IntRegs:$src2), asmstr, pattern> {
  let Inst{16-19} = 0x5; // 8 byte immediate
  let Inst{20-23} = 0x1; // register
  let Constraints = "$src2 = $dst";
}
