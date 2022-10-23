set(LLVM_TARGET_DEFINITIONS FARA.td)

tablegen(LLVM FARAGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM FARAGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM FARAGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM FARAGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM FARAGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM FARAGenCallingConv.inc -gen-callingconv)

add_public_tablegen_target(FARACommonTableGen)

add_llvm_target(FARACodeGen
  FARASubtarget.cpp
  FARATargetMachine.cpp
)

#   FARACodeGen
#   FARAAsmPrinter.cpp
#   FARADelaySlotFiller.cpp
#   FARAFrameLowering.cpp
#   FARAInstrInfo.cpp
#   FARAISelDAGToDAG.cpp
#   FARAISelLowering.cpp
#   FARAMachineFunctionInfo.cpp
#   FARAMCInstLower.cpp
#   FARAMemAluCombiner.cpp
#   FARARegisterInfo.cpp
#   FARASelectionDAGInfo.cpp
#   FARATargetObjectFile.cpp
#)
