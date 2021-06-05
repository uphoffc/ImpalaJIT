#include "std_math_lib.h"
#include "llvm/IR/Module.h"

namespace impala {
namespace engine {

StdMathLib::SupportedFunctionSetT &StdMathLib::getSupportedFunctionSet() {
  static StdMathLib::SupportedFunctionSetT supportedSet{
      {"cos", 1},  {"sin", 1},  {"tan", 1},   {"acos", 1}, {"asin", 1},  {"atan", 1}, {"atan2", 2}, {"cosh", 1},
      {"sinh", 1}, {"tanh", 1}, {"exp", 1},   {"log", 1},  {"log10", 1}, {"pow", 2},  {"sqrt", 1},  {"min", 2},
      {"max", 2},  {"abs", 1},  {"floor", 1}, {"ceil", 1}, {"round", 1}, {"erf", 1},  {"erfc", 1}};
  return supportedSet;
}

StdMathLib::FunctionAliasingMapT &StdMathLib::getFunctionAliasingMap() {
  static StdMathLib::FunctionAliasingMapT aliasingMap{
      {"cos", "cos"},     {"sin", "sin"},   {"tan", "tan"},     {"asin", "asin"}, {"acos", "acos"}, {"atan", "atan"},
      {"atan2", "atan2"}, {"cosh", "cosh"}, {"sinh", "sinh"},   {"tanh", "tanh"}, {"exp", "exp"},   {"log", "log"},
      {"log10", "log10"}, {"pow", "pow"},   {"sqrt", "sqrt"},   {"min", "fmin"},  {"max", "fmax"},  {"abs", "abs"},
      {"floor", "floor"}, {"ceil", "ceil"}, {"round", "round"}, {"erf", "erf"},   {"erfc", "erfc"}};

  return aliasingMap;
}

bool StdMathLib::isSupported(const types::FunctionSinatureT &signature) {
  auto &supportedSet = StdMathLib::getSupportedFunctionSet();
  return supportedSet.find(signature) != supportedSet.end();
}

types::FunctionProtosT StdMathLib::fillModule(std::unique_ptr<llvm::Module> &module, bool isDoublePrecision) {

  auto& context = module->getContext();
  llvm::Type *realType = (isDoublePrecision) ? llvm::Type::getDoubleTy(context) : llvm::Type::getFloatTy(context);
  std::unordered_map<std::string, llvm::Function *> functionProtoTable;

  auto &supportedSet = StdMathLib::getSupportedFunctionSet();
  auto &aliasingMap = StdMathLib::getFunctionAliasingMap();
  for (const auto &signature : supportedSet) {
    const auto &name = signature.first;
    const unsigned numParams = signature.second;
    const std::string &realName = aliasingMap[name];

    std::vector<llvm::Type *> params(numParams, realType);
    llvm::FunctionType *functionType = llvm::FunctionType::get(realType, params, false);
    llvm::Function *function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, realName, *module);

    functionProtoTable[name] = function;
  }

  return functionProtoTable;
}
} // namespace engine
} // namespace impala
