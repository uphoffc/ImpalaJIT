#include "optimizer.h"
#include <iostream>

namespace impala {
namespace engine {
Optimizer::Optimizer() {
  passBuilder.registerModuleAnalyses(moduleAnalysisManager);
  passBuilder.registerCGSCCAnalyses(cGSCCAnalysisManager);
  passBuilder.registerFunctionAnalyses(functionAnalysisManager);
  passBuilder.registerLoopAnalyses(loopAnalysisManager);

  passBuilder.crossRegisterProxies(loopAnalysisManager, functionAnalysisManager, cGSCCAnalysisManager,
                                   moduleAnalysisManager);

  modulePassManager = passBuilder.buildPerModuleDefaultPipeline(llvm::PassBuilder::OptimizationLevel::O1, false);
}

void Optimizer::run(llvm::Module &module) { modulePassManager.run(module, moduleAnalysisManager); }
} // namespace engine
} // namespace impala