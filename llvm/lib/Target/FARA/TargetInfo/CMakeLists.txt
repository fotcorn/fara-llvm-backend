add_llvm_component_library(LLVMFARAInfo
  FARATargetInfo.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  FARA
  )
