#ifndef IMPALA_CPP_STD_MATH_LIB_HPP
#define IMPALA_CPP_STD_MATH_LIB_HPP

#include "hashers.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include <memory>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace impala {
namespace engine {
class StdMathLib {
public:
  static types::FunctionProtosT fillModule(std::unique_ptr<llvm::Module> &module, llvm::Type *realType);
  static bool isSupported(const types::FunctionSinatureT &signature);

private:
  static const std::unordered_set<types::FunctionSinatureT, impala::FunctionSignatureHash> supportedFunctions;
  std::unordered_map<std::string, llvm::Function *> functionTable;
};
} // namespace engine
} // namespace impala

#endif // IMPALA_CPP_STD_MATH_LIB_HPP
