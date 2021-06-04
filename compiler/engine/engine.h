#ifndef IMPALA_CPP_ENGINE_H
#define IMPALA_CPP_ENGINE_H

#include "engine_types.h"
#include "symbol_table.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ExecutionEngine/JITSymbol.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/ExecutionUtils.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/TargetSelect.h"
#include <iostream>
#include <memory>
#include <vector>

/*
 * ExecutionSession - provides context for our running JIT’d code (including the string pool,
 * global mutex, and error reporting facilities)
 *
 * RTDyldObjectLinkingLayer - can be used to add object files to our JIT
 *
 * IRCompileLayer - can be used to add LLVM Modules to our JIT (builds on the ObjectLayer)
 *
 * DataLayout and MangleAndInterner - used for symbol mangling
 *
 * LLVMContext - llvm core
 *
 *
 * return std::make_unique<llvm::SectionMemoryManager>() - builds a JIT memory manager
 * for each module that is added
 *
 * ConcurrentIRCompiler - used the JITTargetMachineBuilder to build llvm TargetMachines
 *
 * JITDylib - A symbol table that supports asynchoronous symbol queries
 * */
namespace impala {
namespace engine {
class Jit {
public:
  struct Toolbox {
    Toolbox(llvm::LLVMContext &context, engine::types::FunctionProtosT &externalMathFunctions)
        : context(context), externalMathFunctions(externalMathFunctions) {
      builder = std::make_unique<llvm::IRBuilder<>>(context);
    }
    llvm::LLVMContext &context;
    engine::types::FunctionProtosT &externalMathFunctions;
    std::unique_ptr<llvm::IRBuilder<>> builder{nullptr};
    SymbolTable symbolTable{};
  };

  static Jit &getJit();

  const llvm::DataLayout &getDataLayout() const { return *dataLayout; }

  llvm::LLVMContext &getContext() { return *context->getContext(); }

  llvm::orc::ThreadSafeContext::Lock getLock() { return context->getLock(); }

  // adds IR to the JIT and making it available for execution
  void addModule(std::unique_ptr<llvm::Module> &module);

  // looks up addresses for function and variable definitions added to the JIT based on their symbol names.
  llvm::JITEvaluatedSymbol lookup(llvm::StringRef Name);

  std::unique_ptr<llvm::Module> createModule();

  types::FunctionProtosT &getExternalMathFunctions() { return externalMathFunctions; }

  Jit::Toolbox createToolbox();

  static void printIRFunction(llvm::Function *function);

  static void printIRModule(llvm::Module &module);

private:
  Jit();
  llvm::orc::ExecutionSession ES;
  std::unique_ptr<llvm::orc::RTDyldObjectLinkingLayer> objectLayer{nullptr};
  std::unique_ptr<llvm::orc::IRCompileLayer> compileLayer{nullptr};
  llvm::orc::JITDylib *jitDylib{nullptr};

  std::unique_ptr<llvm::DataLayout> dataLayout{nullptr};
  std::unique_ptr<llvm::orc::MangleAndInterner> mangle{nullptr};
  std::unique_ptr<llvm::orc::ThreadSafeContext> context{nullptr};
  types::FunctionProtosT externalMathFunctions;
}; // namespace Jit
} // namespace engine
} // namespace impala
#endif // IMPALA_CPP_ENGINE_H
