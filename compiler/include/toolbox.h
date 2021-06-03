#ifndef IMPALA_CPP_STATE_H
#define IMPALA_CPP_STATE_H

#include "engine_types.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Value.h"
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace impala {

struct SymbolTable {
  SymbolTable() = default;

  void removeScope() { scopes.pop_back(); }
  void addScope() { scopes.push_back(TableT{});}
  size_t getNumScopes() {return scopes.size();}

  llvm::Value* findSymbol(std::string name) {
    for (auto scope = scopes.rbegin(); scope != scopes.rend(); ++scope) {
      if (scope->find(name) != scope->end()) {
        return (*scope)[name];
      }
    }
    return nullptr;
  }

  void addSymbol(std::string name, llvm::Value* value) {
    if (scopes.empty()) {
      this->addScope();
    }
    auto& topMostScope = scopes.back();
    if (topMostScope.find(name) != topMostScope.end()) {
      std::stringstream errStream;
      errStream << "symbol `" << name << "` has already been defined in this scope";
      throw std::runtime_error(errStream.str());
    }
    else {
      topMostScope[name] = value;
    }
  }

  using TableT = std::unordered_map<std::string, llvm::Value*>;

private:
  std::vector<TableT> scopes{};
};


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
