#include "engine.h"
#include <memory>

#include "llvm/ExecutionEngine/Orc/LLJIT.h"

Jit& Jit::getJit() {
  static Jit jit;
  return jit;
}

Jit::Jit() {
  LLVMInitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();

  auto expectedJTMB = llvm::orc::JITTargetMachineBuilder::detectHost();
  if (!expectedJTMB) {
    throw std::runtime_error("cannot create jit machine builder");
  }

  auto expectedDataLayout = expectedJTMB->getDefaultDataLayoutForTarget();
  if (!expectedDataLayout) {
    throw std::runtime_error("cannot detect data layout");
  }

  try {
    auto memoryManager = []() { return std::make_unique<llvm::SectionMemoryManager>(); };
    objectLayer = std::make_unique<llvm::orc::RTDyldObjectLinkingLayer>(ES, memoryManager);
    auto compiler = std::make_unique<llvm::orc::ConcurrentIRCompiler>(std::move(*expectedJTMB));
    dataLayout = std::make_unique<llvm::DataLayout>(std::move(*expectedDataLayout));
    mangle = std::make_unique<llvm::orc::MangleAndInterner>(ES, *dataLayout);
    std::string dylibName{"jit_dylib"};
    ES.createJITDylib(dylibName);
    jitDylib = ES.getJITDylibByName(dylibName);
    context = std::make_unique<llvm::orc::ThreadSafeContext>(std::make_unique<llvm::LLVMContext>());
  }
  catch (const std::exception& err) {
    llvm::errs() << err.what() << '\n';
    throw err;
  }
}


void Jit::addModule(std::unique_ptr<llvm::Module>& module) {
  llvm::cantFail(compileLayer->add(*jitDylib, llvm::orc::ThreadSafeModule(std::move(module), *context)));
}


llvm::JITEvaluatedSymbol Jit::lookup(llvm::StringRef Name) {
  // lookup will implicitly trigger compilation for any symbol that has not already been compiled
  auto& m = *mangle;
  llvm::Expected<llvm::JITEvaluatedSymbol> expected = ES.lookup({jitDylib}, (*mangle)(Name.str()));
  if (!expected) {
    llvm::errs() << expected.takeError() << '\n';
  }
  return *expected;
}