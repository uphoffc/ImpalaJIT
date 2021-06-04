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

#ifndef IMPALAJIT_EXTERNAL_FUNCTION_NODE_H
#define IMPALAJIT_EXTERNAL_FUNCTION_NODE_H

#include "node.h"
#include "std_math_lib.h"
#include <sstream>
#include <iostream>

class ExternalFunctionNode: public Node
{
public:
    std::string &name;
    ExternalFunctionNode(std::string &_name, std::vector<Node*> &_parameters)
            : name(_name), Node(EXTERNAL_FUNCTION)
    {
        nodes.insert(nodes.end(), _parameters.begin(), _parameters.end());
    }
    ExternalFunctionNode(std::string &_name)
            : name(_name), Node(EXTERNAL_FUNCTION)
    {
    }
    virtual ~ExternalFunctionNode()
    {
    }

    llvm::Value* codegen(impala::engine::Jit::Toolbox& tools) override {
      std::stringstream errStream;
      if (tools.externalMathFunctions.find(name) == tools.externalMathFunctions.end()) {
        errStream << "impala: function `" << name << "` doesn't belong to the standard math library";
        throw std::runtime_error(errStream.str());
      }

      auto proto = tools.externalMathFunctions[name];
      if (nodes.size() != proto->arg_size()) {
        errStream << "impala: function `" << name
                   << "` takes " << proto->arg_size() << " parameters, "
                   << "given " << nodes.size();
        throw std::runtime_error(errStream.str());
      }

      std::vector<llvm::Value* > args;
      for (auto node: nodes) {
        auto arg = node->codegen(tools);
        if (arg) {
          args.push_back(arg);
        }
        else {
          errStream << "impala: found a statement in a list of formal parameters. "
                 << "Please, check `" << name << "` function call";
          throw std::runtime_error(errStream.str());
        }
      }

      std::cout << "ExternalFunctionNode" << std::endl;
      return tools.builder->CreateCall(proto, args);
    }
};

class ExternalFunctionParametersNode : public Node
{
public:
    ExternalFunctionParametersNode()
            : Node(EXTERNAL_FUNCTION_PARAMETER)
    {
    }

    virtual ~ExternalFunctionParametersNode()
    {
    }

    llvm::Value* codegen(impala::engine::Jit::Toolbox& tools) override {
      std::cout << "ExternalFunctionParametersNode" << std::endl;
      throw std::runtime_error("impala: never been suported by the language");
      return nullptr;
    }
};


#endif //IMPALAJIT_EXTERNAL_FUNCTION_NODE_H