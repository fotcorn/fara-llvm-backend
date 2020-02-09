set(LLVM_TARGET_DEFINITIONS Machine.td)

tablegen(LLVM MachineGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM MachineGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM MachineGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM MachineGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM MachineGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM MachineGenCallingConv.inc -gen-callingconv)

add_public_tablegen_target(MachineCommonTableGen)

add_llvm_target(#MachineCodeGen
#   MachineAsmPrinter.cpp
#   MachineDelaySlotFiller.cpp
#   MachineFrameLowering.cpp
#   MachineInstrInfo.cpp
#   MachineISelDAGToDAG.cpp
#   MachineISelLowering.cpp
#   MachineMachineFunctionInfo.cpp
#   MachineMCInstLower.cpp
#   MachineMemAluCombiner.cpp
#   MachineRegisterInfo.cpp
#   MachineSelectionDAGInfo.cpp
   MachineSubtarget.cpp
   MachineTargetMachine.cpp
#   MachineTargetObjectFile.cpp
)
