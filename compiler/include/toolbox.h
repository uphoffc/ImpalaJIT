#ifndef IMPALA_CPP_STATE_H
#define IMPALA_CPP_STATE_H

#include "engine_types.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Value.h"
#include <string>
#include <unordered_map>

namespace impala {
using TableT = std::unordered_map<std::string, llvm::Value *>;
struct Toolbox {
  Toolbox(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, engine::types::FunctionProtosT &externalMathFunctions)
      : context(context), builder(builder), externalMathFunctions(externalMathFunctions) {}
  Toolbox(const Toolbox &other) = default;
  llvm::IRBuilder<> &builder;
  llvm::LLVMContext &context;
  engine::types::FunctionProtosT &externalMathFunctions;
  TableT table{};
};
} // namespace impala

#endif // IMPALA_CPP_STATE_H
