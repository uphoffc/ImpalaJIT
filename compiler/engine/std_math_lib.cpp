#include "std_math_lib.h"

namespace impala {
namespace engine {
decltype(StdMathLib::supportedFunctions) StdMathLib::supportedFunctions = decltype(supportedFunctions){
    {"cos", 1},  {"sin", 1},  {"tan", 1},   {"acos", 1}, {"asin", 1},  {"atan", 1}, {"atan2", 2}, {"cosh", 1},
    {"sinh", 1}, {"tanh", 1}, {"exp", 1},   {"log", 1},  {"log10", 1}, {"pow", 2},  {"sqrt", 1},  {"min", 2},
    {"max", 2},  {"abs", 1},  {"floor", 1}, {"ceil", 1}, {"round", 1}, {"erf", 1},  {"erfc", 1}};

bool StdMathLib::isSupported(const types::FunctionSinatureT &signature) {
  return StdMathLib::supportedFunctions.find(signature) != StdMathLib::supportedFunctions.end();
}

types::FunctionProtosT StdMathLib::fillModule(std::unique_ptr<llvm::Module> &module, llvm::Type *realType) {
  std::unordered_map<std::string, llvm::Function *> functionProtoTable;

  for (const auto &signature : StdMathLib::supportedFunctions) {
    const auto &name = signature.first;
    const unsigned numParams = signature.second;

    std::vector<llvm::Type *> params(numParams, realType);
    llvm::FunctionType *functionType = llvm::FunctionType::get(realType, params, false);
    llvm::Function *function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, name, *module);

    functionProtoTable[name] = function;
  }

  return functionProtoTable;
}
} // namespace engine
} // namespace impala
