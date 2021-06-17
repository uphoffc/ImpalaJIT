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

#include <impalajit.hh>
#include <engine.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <driver.h>
#include <iterator>

impalajit::Driver driver;

impalajit::Compiler::Compiler() {
    char* fileName = std::getenv(ENV_VARIABLE_NAME);
    if(fileName == NULL) {
        throw std::runtime_error("Please specify env variable IMPALA_FILE");
    }
    loadFunctionDefinitionsFromInputFiles(std::string(fileName));
}

impalajit::Compiler::Compiler(std::string _configFilePath) {
    loadFunctionDefinitionsFromInputFiles(_configFilePath);
}

impalajit::Compiler::Compiler(std::vector<std::string> _functionDefinitions)
    : functionDefinitions(_functionDefinitions) {
}

void impalajit::Compiler::loadFunctionDefinitionsFromInputFiles(std::string _configPath){

    std::ifstream inFile;
    inFile.open(_configPath.c_str());
    if(!inFile.good()) {
        inFile.close();
        throw std::runtime_error("Bad configuration file");
    }

    std::stringstream strStream;
    strStream << inFile.rdbuf();
    inFile.close();

    std::string inputText = strStream.str();

    std::vector<std::string> functionFiles;
    std::size_t start = 0, end = 0;

    while ((end = inputText.find(';', start)) != std::string::npos) {
        functionFiles.push_back(inputText.substr(start, end - start));
        start = end + 1;
    }

    for (auto& file: functionFiles) {
        std::ifstream in(file.c_str());
        functionDefinitions.push_back(std::string((std::istreambuf_iterator<char>(in)),
                                             (std::istreambuf_iterator<char>())));
    }
}

void impalajit::Compiler::compile(Options options) {
  assert(options.isDoublePrecision && "impala: single precision support has not been fully implemented. Please, use double precision");

  auto& jit = impala::engine::Jit::getJit();
  auto currentModule = jit.createModule(options.isDoublePrecision);
  std::vector<FunctionContext::FunctionSinatureT> functionSignature;
  for(auto& definition: functionDefinitions) {
    functionSignature.emplace_back(driver.generateLLVMFunction(definition, *currentModule, options));
    driver.deleteFunctionContext();
  }
  jit.addModule(currentModule, !options.debug);

  for (auto& signature: functionSignature) {
    auto function = reinterpret_cast<dasm_gen_func>(jit.lookup(signature.first).getAddress());
    functionMap[signature.first] = function;
    parameterCountMap[signature.first] = signature.second;
  }
}

dasm_gen_func impalajit::Compiler::getFunction(std::string functionName) {
    if(functionMap.find(functionName) == functionMap.end()){
        throw std::runtime_error("Function \""+functionName+"\" not found");
    }
    return functionMap.at(functionName);
}

unsigned int impalajit::Compiler::getParameterCount(std::string functionName) {
    if(functionMap.find(functionName) == functionMap.end()){
        throw std::runtime_error("Function \""+functionName+"\" not found");
    }
    return parameterCountMap.at(functionName);
}

void impalajit::Compiler::close() {
    driver.~Driver();
}

//C Interface

impalajit_compiler *impalajit_compiler_create() {
    return new impalajit::Compiler();
}

impalajit_compiler *impalajit_compiler_create_with_config(const char* config_file_path) {
    return new impalajit::Compiler(std::string (config_file_path));
}

impalajit_compiler *impalajit_compiler_create_with_function_definitions(char** function_definitions) {
    std::vector<std::string> lFunctionDefintions;
    for(int i = 0; i< sizeof(function_definitions); i++){
        lFunctionDefintions.push_back(std::string(function_definitions[i]));
    }
    return new impalajit::Compiler(lFunctionDefintions);
}

void impalajit_compiler_compile(impalajit_compiler *handle) {
    handle->compile();
}

unsigned int impalajit_compiler_get_parameter_count(impalajit_compiler *handle, const char* function_name) {
    return handle->getParameterCount(std::string(function_name));
}


dasm_gen_func impalajit_compiler_get_function(impalajit_compiler *handle, const char* function_name){
    return handle->getFunction(std::string(function_name));
}

void impalajit_compiler_close(impalajit_compiler *handle){
    handle->close();
}

