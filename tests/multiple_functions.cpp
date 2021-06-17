#include "definitions.h"
#include "impalajit.hh"
#include "gtest/gtest.h"
#include <fstream>

TEST(StressTest, MultipleFunctions) {
  std::ofstream configFile;
  configFile.open(CONFIG_FILE_PATH);
  configFile << "./impala_files/conditional_eq.impala;";
  configFile << "./impala_files/conditional_gt.impala;";
  configFile << "./impala_files/conditional_gte.impala;";
  configFile << "./impala_files/conditional_lt.impala;";
  configFile << "./impala_files/conditional_lte.impala;";
  configFile << "./impala_files/conditional_neq.impala;";
  configFile.close();

  impalajit::Compiler compiler(CONFIG_FILE_PATH);
  compiler.compile();

  ASSERT_EQ(compiler.getFunction("eq")(1.0, 1.0), 1.0);
  ASSERT_EQ(compiler.getFunction("eq")(1.0, 0.0), 0.0);

  ASSERT_EQ(compiler.getFunction("gt")(1.0, 0.0), 1.0);
  ASSERT_EQ(compiler.getFunction("gt")(0.0, 1.0), 0.0);
  ASSERT_EQ(compiler.getFunction("gt")(0.0, 0.0), 0.0);

  ASSERT_EQ(compiler.getFunction("gte")(2.0, 1.0), 1.0);
  ASSERT_EQ(compiler.getFunction("gte")(1.0, 1.0), 1.0);
  ASSERT_EQ(compiler.getFunction("gte")(0.0, 1.0), 0.0);

  ASSERT_EQ(compiler.getFunction("lt")(1.0, 0.0), 0.0);
  ASSERT_EQ(compiler.getFunction("lt")(0.0, 1.0), 1.0);
  ASSERT_EQ(compiler.getFunction("lt")(0.0, 0.0), 0.0);

  ASSERT_EQ(compiler.getFunction("lte")(2.0, 1.0), 0.0);
  ASSERT_EQ(compiler.getFunction("lte")(1.0, 1.0), 1.0);
  ASSERT_EQ(compiler.getFunction("lte")(0.0, 1.0), 1.0);

  ASSERT_EQ(compiler.getFunction("neq")(2.0, 1.0), 1.0);
  ASSERT_EQ(compiler.getFunction("neq")(1.0, 1.0), 0.0);
  ASSERT_EQ(compiler.getFunction("neq")(0.0, 1.0), 1.0);
  compiler.close();
}