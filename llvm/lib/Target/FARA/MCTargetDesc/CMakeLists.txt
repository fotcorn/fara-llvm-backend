add_llvm_component_library(LLVMFARADesc
  FARAMCTargetDesc.cpp
  FARAMCAsmInfo.cpp
  FARATargetStreamer.cpp
  FARAAsmBackend.cpp
  FARAELFObjectWriter.cpp

  LINK_COMPONENTS
  MC
  FARAInfo
  Support

  ADD_TO_COMPONENT
  FARA
  )
