//
// Created by manuel on 28.01.17.
//

#include "impalajit.hh"
#include <iostream>
#include <fstream>
#include <assert.h>
#include <defines.hh>
using namespace std;

int main(int argc, char** argv) {

    ofstream configFile;
    configFile.open(CONFIG_FILE_PATH);
    configFile << "../../tests/impala_files/conditional_eq.impala\n";
    configFile << "../../tests/impala_files/conditional_gt.impala\n";
    configFile << "../../tests/impala_files/conditional_gte.impala\n";
    configFile << "../../tests/impala_files/conditional_lt.impala\n";
    configFile << "../../tests/impala_files/conditional_lte.impala\n";
    configFile << "../../tests/impala_files/conditional_neq.impala";
    configFile.close();

    impalajit::Compiler compiler(CONFIG_FILE_PATH);
    compiler.compile();

    assert(compiler.getFunction("eq")(1.0,1.0) == 1.0);
    assert(compiler.getFunction("eq")(1.0,0.0) == 0.0);

    assert(compiler.getFunction("gt")(1.0, 0.0) == 1.0);
    assert(compiler.getFunction("gt")(0.0, 1.0) == 0.0);
    assert(compiler.getFunction("gt")(0.0, 0.0) == 0.0);

    assert(compiler.getFunction("gte")(2.0, 1.0) == 1.0);
    assert(compiler.getFunction("gte")(1.0, 1.0) == 1.0);
    assert(compiler.getFunction("gte")(0.0, 1.0) == 0.0);

    assert(compiler.getFunction("lt")(1.0, 0.0) == 0.0);
    assert(compiler.getFunction("lt")(0.0, 1.0) == 1.0);
    assert(compiler.getFunction("lt")(0.0, 0.0) == 0.0);

    assert(compiler.getFunction("lte")(2.0, 1.0) == 0.0);
    assert(compiler.getFunction("lte")(1.0, 1.0) == 1.0);
    assert(compiler.getFunction("lte")(0.0, 1.0) == 1.0);

    assert(compiler.getFunction("neq")(2.0, 1.0) == 1.0);
    assert(compiler.getFunction("neq")(1.0, 1.0) == 0.0);
    assert(compiler.getFunction("neq")(0.0, 1.0) == 1.0);
    return 0;
}