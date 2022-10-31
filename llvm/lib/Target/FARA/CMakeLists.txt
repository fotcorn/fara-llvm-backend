add_llvm_component_group(FARA)

set(LLVM_TARGET_DEFINITIONS FARA.td)

tablegen(LLVM FARAGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM FARAGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM FARAGenCallingConv.inc -gen-callingconv)
tablegen(LLVM FARAGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM FARAGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM FARAGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM FARAGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM FARAGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM FARAGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(FARACommonTableGen)

add_llvm_target(FARACodeGen
  FARASubtarget.cpp
  FARATargetMachine.cpp
  FARAInstrInfo.cpp

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  CodeGen
  Core
  MC
  FARADesc
  FARAInfo
  IPO
  Scalar
  SelectionDAG
  Support
  Target
  TransformUtils

  ADD_TO_COMPONENT
  FARA
  )

add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
