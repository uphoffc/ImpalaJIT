#include "definitions.h"
#include "impalajit.hh"
#include "gtest/gtest.h"
#include <fstream>

TEST(ComplexConditions, Version1) {

  std::ofstream configFile;
  configFile.open(CONFIG_FILE_PATH);
  configFile << "./impala_files/conditional_complex_1.impala;";
  configFile.close();

  impalajit::Compiler compiler(CONFIG_FILE_PATH);
  compiler.compile();
  dasm_gen_func function = compiler.getFunction("complex_1");
  compiler.close();

  ASSERT_EQ(function(1.0, 1.0, 1.0, 1.0), 1.0);
  ASSERT_EQ(function(1.0, 1.0, 1.0, 0.0), 1.0);
  ASSERT_EQ(function(1.0, 1.0, 0.0, 1.0), 1.0);
  ASSERT_EQ(function(1.0, 1.0, 0.0, 0.0), 1.0);
  ASSERT_EQ(function(1.0, 0.0, 1.0, 1.0), 1.0);
  ASSERT_EQ(function(1.0, 0.0, 1.0, 0.0), 2.0);
  ASSERT_EQ(function(1.0, 0.0, 0.0, 1.0), 2.0);
  ASSERT_EQ(function(1.0, 0.0, 0.0, 0.0), 2.0);
  ASSERT_EQ(function(0.0, 1.0, 1.0, 1.0), 2.0);
  ASSERT_EQ(function(0.0, 1.0, 1.0, 0.0), 2.0);
  ASSERT_EQ(function(0.0, 1.0, 0.0, 1.0), 2.0);
  ASSERT_EQ(function(0.0, 1.0, 0.0, 0.0), 2.0);
  ASSERT_EQ(function(0.0, 0.0, 1.0, 1.0), 2.0);
  ASSERT_EQ(function(0.0, 0.0, 1.0, 0.0), 2.0);
  ASSERT_EQ(function(0.0, 0.0, 0.0, 1.0), 2.0);
  ASSERT_EQ(function(0.0, 0.0, 0.0, 0.0), 2.0);
}

TEST(ComplexConditions, Version2) {
  std::ofstream configFile;
  configFile.open(CONFIG_FILE_PATH);
  configFile << "./impala_files/conditional_complex_2.impala;";
  configFile.close();

  impalajit::Compiler compiler(CONFIG_FILE_PATH);
  compiler.compile();
  dasm_gen_func function = compiler.getFunction("complex_2");
  compiler.close();

  ASSERT_EQ(function(1.0, 1.0, 1.0, 1.0), 2.0);
  ASSERT_EQ(function(1.0, 1.0, 1.0, 0.0), 2.0);
  ASSERT_EQ(function(1.0, 1.0, 0.0, 1.0), 2.0);
  ASSERT_EQ(function(1.0, 1.0, 0.0, 0.0), 2.0);
  ASSERT_EQ(function(1.0, 0.0, 1.0, 1.0), 2.0);
  ASSERT_EQ(function(1.0, 0.0, 1.0, 0.0), 2.0);
  ASSERT_EQ(function(1.0, 0.0, 0.0, 1.0), 2.0);
  ASSERT_EQ(function(1.0, 0.0, 0.0, 0.0), 2.0);
  ASSERT_EQ(function(0.0, 1.0, 1.0, 1.0), 1.0);
  ASSERT_EQ(function(0.0, 1.0, 1.0, 0.0), 2.0);
  ASSERT_EQ(function(0.0, 1.0, 0.0, 1.0), 1.0);
  ASSERT_EQ(function(0.0, 1.0, 0.0, 0.0), 2.0);
  ASSERT_EQ(function(0.0, 0.0, 1.0, 1.0), 1.0);
  ASSERT_EQ(function(0.0, 0.0, 1.0, 0.0), 2.0);
  ASSERT_EQ(function(0.0, 0.0, 0.0, 1.0), 2.0);
  ASSERT_EQ(function(0.0, 0.0, 0.0, 0.0), 2.0);
}

TEST(ComplexConditions, NestedContions) {
  std::ofstream configFile;
  configFile.open(CONFIG_FILE_PATH);
  configFile << "./impala_files/conditional_nested.impala;";
  configFile.close();

  impalajit::Compiler compiler(CONFIG_FILE_PATH);
  compiler.compile();
  dasm_gen_func function = compiler.getFunction("nested");
  compiler.close();

  ASSERT_EQ(function(1.0, 1.0, 1.0, 1.0), 4.0);
  ASSERT_EQ(function(1.0, 1.0, 1.0, 0.0), 3.0);
  ASSERT_EQ(function(1.0, 0.0, 1.0, 1.0), 2.0);
  ASSERT_EQ(function(1.0, 0.0, 1.0, 0.0), 1.0);
}