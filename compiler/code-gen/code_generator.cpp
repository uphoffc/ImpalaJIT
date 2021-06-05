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

#include "code_generator.h"
#include "codegen_visitor.h"
#include "engine.h"
#include "pretty_printer.h"
#include "llvm/IR/Verifier.h"
#include <stdexcept>

void CodeGenerator::generateLLVMCode(FunctionContext *&functionContext, llvm::Module &module,
                                     const impalajit::Options &options) {
  if (options.printAST) {
    impala::PrettyPrinter printer;
    functionContext->root->accept(&printer);
    std::cout << std::string(80, '=') << std::endl;
  }

  auto &jit = impala::engine::Jit::getJit();
  {
    auto lock = jit.getLock();
    impala::engine::Jit::Toolbox tools = jit.createToolbox();
    llvm::Type *realType =
        (options.isDoublePrecision) ? llvm::Type::getDoubleTy(tools.context) : llvm::Type::getFloatTy(tools.context);

    auto function = CodeGenerator::genFunctionProto(functionContext, module, tools, realType);
    impala::CodegenVisitor codegenVisitor(tools, realType);
    functionContext->root->accept(&codegenVisitor);

    llvm::verifyFunction(*function, &llvm::outs());
    if (options.printIR) {
      impala::engine::Jit::printIRFunction(function);
    }
  }
}

llvm::Function *CodeGenerator::genFunctionProto(FunctionContext *&functionContext, llvm::Module &currModule,
                                                impala::engine::Jit::Toolbox &tools, llvm::Type *realType) {

  const auto numParams = functionContext->parameters.size();
  std::vector<llvm::Type *> paramTypes(numParams, realType);
  auto functionProto = llvm::FunctionType::get(realType, paramTypes, false);

  // First, see if the function has already been added to the current module
  // check: name, type of operands and a number of operands
  if (auto previouslyDefined = currModule.getFunction(functionContext->name)) {
    bool isSame = (previouslyDefined->getName() == llvm::StringRef(functionContext->name));
    // type comparison is done via pointer comparison in LLVM
    isSame &= (previouslyDefined->getNumOperands() == functionContext->parameters.size());
    isSame &= (previouslyDefined->getFunctionType() == realType);

    if (isSame) {
      std::stringstream errStream;
      errStream << "function `" << functionContext->name << "` has been previously defined";
      throw std::runtime_error(errStream.str());
    }
  }

  auto function =
      llvm::Function::Create(functionProto, llvm::Function::ExternalLinkage, functionContext->name, currModule);

  auto entryBlock = llvm::BasicBlock::Create(tools.context, "entry", function);
  tools.builder->SetInsertPoint(entryBlock);

  const auto &params = functionContext->parameters;
  for (size_t i = 0; i < params.size(); ++i) {
    auto arg = function->getArg(i);
    arg->setName(params[i]);

    auto argPtr = tools.builder->CreateAlloca(realType);
    tools.symbolTable.addSymbol(params[i], argPtr);
    tools.builder->CreateStore(arg, argPtr);
  }

  return function;
}