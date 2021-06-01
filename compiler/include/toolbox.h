#ifndef IMPALA_CPP_STATE_H
#define IMPALA_CPP_STATE_H

#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"
#include <string>
#include <unordered_map>

namespace impala {
using TableT = std::unordered_map<std::string, llvm::Value *>;
struct Toolbox {
  Toolbox(llvm::LLVMContext& context, llvm::IRBuilder<> &builder) : context(context), builder(builder) {}
  Toolbox(const Toolbox& other) = default;
  llvm::IRBuilder<> &builder;
  llvm::LLVMContext &context;
  TableT table{};
};
}

#endif //IMPALA_CPP_STATE_H
