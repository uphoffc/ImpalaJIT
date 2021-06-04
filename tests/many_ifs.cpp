#include "definitions.h"
#include "impalajit.hh"
#include "gtest/gtest.h"
#include <fstream>
#include <limits>

namespace impala {
namespace tests {
namespace many_conditions {
double reference_function(double z, double i) {
  if (z < 10000.0 && z >= 500.0) {
    if (i == 1.0) {
      return 2100.0;
    }
    if (i == 2.0) {
      return 2100000000.0;
    }
    if (i == 3.0) {
      return 3381000000.0;
    }
  }
  if (z < -500.0 && z >= -1500.0) {
    if (i == 1.0) {
      return 2400.0;
    }
    if (i == 2.0) {
      return 9600000000.0;
    }
    if (i == 3.0) {
      return 19200000000.0;
    }
  }
  if (z < -1500.0 && z >= -4000.0) {
    if (i == 1.0) {
      return 2700.0;
    }
    if (i == 2.0) {
      return 27648000000.0;
    }
    if (i == 3.0) {
      return 26379000000.0;
    }
  }
  if (z < -4000.0 && z >= -270000.0) {
    if (i == 1.0) {
      return 2800.0;
    }
    if (i == 2.0) {
      return 36288000000.0;
    }
    if (i == 3.0) {
      return 38556000000.0;
    }
  }
  if (z < -270000.0 && z >= -400000.0) {
    if (i == 1.0) {
      return 2900.0;
    }
    if (i == 2.0) {
      return 44109000000.0;
    }
    if (i == 3.0) {
      return 45878000000.0;
    }
  }
  if (i == 1.0) {
    return 3300.0;
  }
  if (i == 2.0) {
    return 66825000000.0;
  }
  if (i == 3.0) {
    return 67122000000.0;
  }
  return std::numeric_limits<double>::min();
}
} // namespace many_conditions
} // namespace tests
} // namespace impala

TEST(StressTest, ManyConditions) {
  std::ofstream configFile;
  configFile.open(CONFIG_FILE_PATH);
  configFile << "./impala_files/many_if.impala;";
  configFile.close();

  impalajit::Compiler compiler(CONFIG_FILE_PATH);
  compiler.compile();
  dasm_gen_func function = compiler.getFunction("many_if");
  compiler.close();

  using namespace impala::tests::many_conditions;
  for (int i = -500000; i < 500000; i++) {
    ASSERT_EQ(function(static_cast<double>(i), 1.0), reference_function(static_cast<double>(i), 1.0));
    ASSERT_EQ(function(static_cast<double>(i), 2.0), reference_function(static_cast<double>(i), 2.0));
    ASSERT_EQ(function(static_cast<double>(i), 3.0), reference_function(static_cast<double>(i), 3.0));
  }
}