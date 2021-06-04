#ifndef IMPALA_CPP_STATE_H
#define IMPALA_CPP_STATE_H

#include "engine_types.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Value.h"
#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace impala {

struct SymbolTable {
  SymbolTable() = default;

  void removeScope() { scopes.pop_back(); }
  void addScope() { scopes.emplace_back(TableT{}); }
  size_t getNumScopes() { return scopes.size(); }

  llvm::Value *operator[](const std::string &name) {
    for (auto scope = scopes.rbegin(); scope != scopes.rend(); ++scope) {
      if (scope->find(name) != scope->end()) {
        return (*scope)[name];
      }
    }
    return nullptr;
  }

  void addSymbol(const std::string &name, llvm::Value *value) {
    if (scopes.empty()) {
      this->addScope();
    }
    auto &topMostScope = scopes.back();
    if (topMostScope.find(name) != topMostScope.end()) {
      std::stringstream errStream;
      errStream << "symbol `" << name << "` has already been defined in this scope";
      throw std::runtime_error(errStream.str());
    } else {
      topMostScope[name] = value;
    }
  }

private:
  using TableT = std::unordered_map<std::string, llvm::Value *>;
  std::vector<TableT> scopes{};
};
} // namespace impala

#endif // IMPALA_CPP_STATE_H
