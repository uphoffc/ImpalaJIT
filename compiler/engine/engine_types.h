#ifndef IMPALA_CPP_ENGINE_TYPES_H
#define IMPALA_CPP_ENGINE_TYPES_H

#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/IR/Function.h"
#include <unordered_map>

namespace impala {
namespace engine {
namespace types {
using FunctionProtosT = std::unordered_map<std::string, llvm::Function *>;
using FunctionSinatureT = std::pair<std::string, unsigned>;
} // namespace types
} // namespace engine
}; // namespace impala

#endif // IMPALA_CPP_ENGINE_TYPES_H
