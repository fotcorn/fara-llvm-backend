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
