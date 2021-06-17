#include "definitions.h"
#include "impalajit.hh"
#include "gtest/gtest.h"
#include <cmath>
#include <fstream>

namespace impala {
namespace tests {
namespace assignment {
double reference_function(double x, double y) {
  double a = 3 * x + 5 * y;
  a = a + 1.0;
  return a;
}
} // namespace assignment

namespace math {
double reference_function(double x, double y) {
  return std::pow(x, y) + std::pow(std::sqrt(3.01 * x + (-y + 12.65)), 2.54) / 3.4331;
}
} // namespace math
} // namespace tests
} // namespace impala

TEST(Basic, Assignment) {
  std::ofstream configFile;
  configFile.open(CONFIG_FILE_PATH);
  configFile << "./impala_files/assignment.impala;";
  configFile.close();

  impalajit::Compiler compiler(CONFIG_FILE_PATH);
  compiler.compile();
  dasm_gen_func function = compiler.getFunction("assignment");
  compiler.close();

  using namespace impala::tests;
  ASSERT_EQ(function(45.3, 43.6), assignment::reference_function(45.3, 43.6));
  ASSERT_EQ(function(-23.3, 456.6), assignment::reference_function(-23.3, 456.6));
}

TEST(Basic, Math) {
  std::ofstream configFile;
  configFile.open(CONFIG_FILE_PATH);
  configFile << "./impala_files/math_expression.impala;";
  configFile.close();

  impalajit::Compiler compiler(CONFIG_FILE_PATH);
  compiler.compile();
  dasm_gen_func function = compiler.getFunction("math_expression");
  compiler.close();

  using namespace impala::tests;
  ASSERT_EQ(function(2.54, -4.21), math::reference_function(2.54, -4.21));
  ASSERT_EQ(function(212.421, -232.22), math::reference_function(212.421, -232.22));
}