#ifndef IMPALAJIT_OPTIMIZER_H
#define IMPALAJIT_OPTIMIZER_H

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"

namespace impala {
namespace engine {
class Optimizer {
public:
  Optimizer();

  void run(llvm::Module &module);

private:
  llvm::PassBuilder passBuilder;
  llvm::LoopAnalysisManager loopAnalysisManager;
  llvm::FunctionAnalysisManager functionAnalysisManager;
  llvm::CGSCCAnalysisManager cGSCCAnalysisManager;
  llvm::ModuleAnalysisManager moduleAnalysisManager;
  llvm::ModulePassManager modulePassManager;
};
} // namespace engine
} // namespace impala

#endif // IMPALAJIT_OPTIMIZER_H
