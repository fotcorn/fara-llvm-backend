include "FARAInstrFormats.td"


def add8s : FARAInst<1, 8, (outs IntRegs:$dst),
        (ins IntRegs:$src1, IntRegs:$src2),
        "add8s $src1, $src2, $dst",
        [(set i64:$dst, (add i64:$src1, i64:$src2))]>;

/*
def add4s : FARAInst<1, 4, (outs IntRegs:$dst),
        (ins IntRegs:$src1, IntRegs:$src2),
        "add4s $src1, $src2, $dst",
        [(set i32:$dst, (add i32:$src1, i32:$src2))]>;
*/
