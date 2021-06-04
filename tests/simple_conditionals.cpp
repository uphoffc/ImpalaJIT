#include "definitions.h"
#include "impalajit.hh"
#include "gtest/gtest.h"
#include <fstream>

TEST(SimpleConditions, Equal) {
  std::ofstream configFile;
  configFile.open(CONFIG_FILE_PATH);
  configFile << "./impala_files/conditional_eq.impala;";
  configFile.close();

  impalajit::Compiler compiler(CONFIG_FILE_PATH);
  compiler.compile();
  dasm_gen_func function = compiler.getFunction("eq");
  compiler.close();

  ASSERT_EQ(function(1.0, 1.0), 1.0);
  ASSERT_EQ(function(1.0, 0.0), 0.0);
}

TEST(SimpleConditions, GreaterThan) {
  std::ofstream configFile;
  configFile.open(CONFIG_FILE_PATH);
  configFile << "./impala_files/conditional_gt.impala;";
  configFile.close();

  impalajit::Compiler compiler(CONFIG_FILE_PATH);
  compiler.compile();
  dasm_gen_func function = compiler.getFunction("gt");
  compiler.close();

  ASSERT_EQ(function(1.0, 0.0), 1.0);
  ASSERT_EQ(function(0.0, 1.0), 0.0);
  ASSERT_EQ(function(0.0, 0.0), 0.0);
}

TEST(SimpleConditions, GreaterThanOrEqual) {
  std::ofstream configFile;
  configFile.open(CONFIG_FILE_PATH);
  configFile << "./impala_files/conditional_gte.impala;";
  configFile.close();

  impalajit::Compiler compiler(CONFIG_FILE_PATH);
  compiler.compile();
  dasm_gen_func function = compiler.getFunction("gte");
  compiler.close();

  ASSERT_EQ(function(2.0, 1.0), 1.0);
  ASSERT_EQ(function(1.0, 1.0), 1.0);
  ASSERT_EQ(function(0.0, 1.0), 0.0);
}

TEST(SimpleConditions, LessThan) {
  std::ofstream configFile;
  configFile.open(CONFIG_FILE_PATH);
  configFile << "./impala_files/conditional_lt.impala;";
  configFile.close();

  impalajit::Compiler compiler(CONFIG_FILE_PATH);
  compiler.compile();
  dasm_gen_func function = compiler.getFunction("lt");
  compiler.close();

  ASSERT_EQ(function(1.0, 0.0), 0.0);
  ASSERT_EQ(function(0.0, 1.0), 1.0);
  ASSERT_EQ(function(0.0, 0.0), 0.0);
}

TEST(SimpleConditions, LessThanOrEqual) {
  std::ofstream configFile;
  configFile.open(CONFIG_FILE_PATH);
  configFile << "./impala_files/conditional_lte.impala;";
  configFile.close();

  impalajit::Compiler compiler(CONFIG_FILE_PATH);
  compiler.compile();
  dasm_gen_func function = compiler.getFunction("lte");
  compiler.close();

  ASSERT_EQ(function(2.0, 1.0), 0.0);
  ASSERT_EQ(function(1.0, 1.0), 1.0);
  ASSERT_EQ(function(0.0, 1.0), 1.0);
}

TEST(SimpleConditions, NotEqual) {
  std::ofstream configFile;
  configFile.open(CONFIG_FILE_PATH);
  configFile << "./impala_files/conditional_neq.impala;";
  configFile.close();

  impalajit::Compiler compiler(CONFIG_FILE_PATH);
  compiler.compile();
  dasm_gen_func function = compiler.getFunction("neq");
  compiler.close();

  ASSERT_EQ(function(2.0, 1.0), 1.0);
  ASSERT_EQ(function(1.0, 1.0), 0.0);
  ASSERT_EQ(function(0.0, 1.0), 1.0);
}