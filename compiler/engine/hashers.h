#ifndef IMPALA_CPP_HASHERS_H
#define IMPALA_CPP_HASHERS_H

#include "engine_types.h"
#include <functional>
#include <limits>
#include <utility>

namespace impala {
template<class T>
inline void hashValue(std::size_t &seed, const T &value) {
  std::hash <T> hasher;
  seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}


template <class T1, class T2>
struct PairHash {
  std::size_t operator()(std::pair<T1, T2> const &instance) const {
    std::size_t result = 0;
    hashValue(result, instance.first);
    hashValue(result, instance.second);
    return result;
  }
};

struct FunctionSignatureHash {
  std::size_t operator()(engine::types::FunctionSinatureT const &instance) const {
    std::size_t result = 0;
    hashValue(result, instance.first);
    hashValue(result, instance.second);
    return result;
  }
};
}

#endif // IMPALA_CPP_HASHERS_H
