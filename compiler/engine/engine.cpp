#include "engine.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/IRPrintingPasses.h"
#include <memory>


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
    throw std::runtime_error("impala: cannot create jit machine builder");
  }

  auto expectedDataLayout = expectedJTMB->getDefaultDataLayoutForTarget();
  if (!expectedDataLayout) {
    throw std::runtime_error("impala: cannot detect data layout");
  }

  try {
    // create memory manager
    auto memoryManager = []() { return std::make_unique<llvm::SectionMemoryManager>(); };

    // create compiler layer
    objectLayer = std::make_unique<llvm::orc::RTDyldObjectLinkingLayer>(ES, memoryManager);
    using compilerLayerT = llvm::orc::IRCompileLayer;
    std::unique_ptr<compilerLayerT::IRCompiler> compiler(new llvm::orc::ConcurrentIRCompiler(std::move(*expectedJTMB)));
    compileLayer = std::make_unique<compilerLayerT>(ES, *objectLayer, std::move(compiler));

    // create data layout
    dataLayout = std::make_unique<llvm::DataLayout>(std::move(*expectedDataLayout));

    // create mangler
    mangle = std::make_unique<llvm::orc::MangleAndInterner>(ES, *dataLayout);

    // runtime dyn. layer
    const std::string dylibName{"jit_dylib"};
    ES.createJITDylib(dylibName);
    jitDylib = ES.getJITDylibByName(dylibName);

    // create llvm context
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
    throw std::runtime_error("impala: lookup error");
  }
  return *expected;
}

void Jit::reserveModule() {
  static long long counter{0};
  std::string moduleName{"impala_module_" + std::to_string(counter)};
  module = (std::make_unique<llvm::Module>(std::move(moduleName), *context->getContext()));
  ++counter;
}

void Jit::registerModule() {
  this->addModule(module);
}

std::unique_ptr<llvm::Module>& Jit::getCurrentModule() {
  return module;
}

void Jit::printIRFunction(llvm::Function* function) {
  llvm::raw_fd_ostream stream(fileno(stdout), false);
  llvm::FunctionPass* printPass = llvm::createPrintFunctionPass(llvm::outs());
  printPass->runOnFunction(*function);
}


void Jit::printIRModule(llvm::Module& module) {
  llvm::raw_fd_ostream stream(fileno(stdout), false);
  llvm::ModulePass* printPass = llvm::createPrintModulePass(llvm::outs());
  printPass->runOnModule(module);
}