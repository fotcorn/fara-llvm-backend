add_llvm_component_library(LLVMFARAAsmParser
  FARAAsmParser.cpp

  LINK_COMPONENTS
  MC
  MCParser
  FARADesc
  FARAInfo
  Support

  ADD_TO_COMPONENT
  FARA
  )
