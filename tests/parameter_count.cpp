#include "definitions.h"
#include "impalajit.hh"
#include "gtest/gtest.h"
#include <fstream>

TEST(FunctionParameters, Count) {
  std::ofstream configFile;
  configFile.open(CONFIG_FILE_PATH);
  configFile << "./impala_files/four_params.impala;";
  configFile << "./impala_files/seven_params.impala;";
  configFile.close();

  impalajit::Compiler compiler(CONFIG_FILE_PATH);
  compiler.compile();

  ASSERT_EQ(compiler.getParameterCount("four_params"), 4);
  ASSERT_EQ(compiler.getParameterCount("seven_params"), 7);

  compiler.close();
}