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

#ifndef IMPALAJIT_ASSIGNMENT_EXPRESSION_HH
#define IMPALAJIT_ASSIGNMENT_EXPRESSION_HH

#include <node.h>

class AssignmentNode : public Node
{
public:
    std::string &name;
    AssignmentNode(std::string &_name, Node* _node)
            : name(_name), Node(ASSIGNMENT)
    {
        nodes.push_back(_node);
    }

    virtual ~AssignmentNode()
    {
    }


    llvm::Value* codegen(impala::Toolbox& tools) override {
      assert(nodes.size() == 1 && "AssignmentNode must have one child expr");
      auto expr = nodes[0]->codegen(tools);

      auto address = tools.symbolTable[name];
      if (!address) {
        auto realType = llvm::Type::getDoubleTy(tools.context);
        address = tools.builder.CreateAlloca(realType);
        tools.symbolTable.addSymbol(name, address);
      }
      std::cout << "AssignmentNode" << std::endl;
      tools.builder.CreateStore(expr, address);
      return nullptr;
    }
};
#endif //IMPALAJIT_ASSIGNMENT_EXPRESSION_HH
