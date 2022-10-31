def CC_FARA: CallingConv<[
  // Promote i1/i8/i16/i32 arguments to i64.
  CCIfType<[i1, i8, i16, i32], CCPromoteToType<i64>>,

  CCIfType<[i64], CCAssignToReg<[I0, I1, I2, I3, I4, I5, I6, I7]>>,

  CCAssignToStack<8, 8>
]>;

def RetCC_FARA : CallingConv<[
  // Promote i1/i8/i16/i32 return values to i64.
  CCIfType<[i1, i8, i16, i32], CCPromoteToType<i64>>,

  CCIfType<[i64], CCAssignToReg<[I0, I1, I2, I3, I4, I5, I6, I7]>>,
]>;
