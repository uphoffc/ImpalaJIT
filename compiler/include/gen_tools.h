#ifndef IMPALA_CPP_STATE_H
#define IMPALA_CPP_STATE_H

#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"
#include <string>
#include <unordered_map>

namespace impala {
using TableT = std::unordered_map<std::string, llvm::Value *>;
struct GenTools {
  GenTools(llvm::LLVMContext& context) : context(context) {}
  GenTools(const GenTools& other) = default;
  llvm::IRBuilder<> *builder{nullptr};
  llvm::LLVMContext &context;
  TableT table{};
};
}

#endif //IMPALA_CPP_STATE_H
