#include "optimizer.h"
#include <iostream>

#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/InstSimplifyPass.h"
#include "llvm/Transforms/Scalar/NewGVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"

namespace impala {
namespace engine {
Optimizer::Optimizer() {
  passBuilder.registerModuleAnalyses(moduleAnalysisManager);
  passBuilder.registerCGSCCAnalyses(cGSCCAnalysisManager);
  passBuilder.registerFunctionAnalyses(functionAnalysisManager);
  passBuilder.registerLoopAnalyses(loopAnalysisManager);

  passBuilder.crossRegisterProxies(loopAnalysisManager, functionAnalysisManager, cGSCCAnalysisManager,
                                   moduleAnalysisManager);

  llvm::FunctionPassManager functionPassManager;
  functionPassManager.addPass(llvm::InstCombinePass());
  functionPassManager.addPass(llvm::NewGVNPass());
  functionPassManager.addPass(llvm::InstSimplifyPass());
  functionPassManager.addPass(llvm::SimplifyCFGPass());

  modulePassManager.addPass(llvm::createModuleToFunctionPassAdaptor(std::move(functionPassManager)));
}

void Optimizer::run(llvm::Module &module) { modulePassManager.run(module, moduleAnalysisManager); }
} // namespace engine
} // namespace impala