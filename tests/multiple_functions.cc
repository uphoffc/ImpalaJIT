/**
 * Copyright 2017 Manuel Fasching <manuel.fasching@tum.de>
 * Distributed under the MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "impalajit.hh"
#include <iostream>
#include <fstream>
#include <assert.h>
#include <defines.hh>
using namespace std;

int main(int argc, char** argv) {

    ofstream configFile;
    configFile.open(CONFIG_FILE_PATH);
    configFile << "../../tests/impala_files/conditional_eq.impala;";
    configFile << "../../tests/impala_files/conditional_gt.impala;";
    configFile << "../../tests/impala_files/conditional_gte.impala;";
    configFile << "../../tests/impala_files/conditional_lt.impala;";
    configFile << "../../tests/impala_files/conditional_lte.impala;";
    configFile << "../../tests/impala_files/conditional_neq.impala;";
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

    std::cout << compiler.getParameterCount("neq") << std::endl;

    compiler.close();
    return 0;
}