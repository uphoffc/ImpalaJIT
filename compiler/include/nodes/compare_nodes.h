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

#ifndef IMPALAJIT_COMPARISON_EXPRESSION_H
#define IMPALAJIT_COMPARISON_EXPRESSION_H

#include <node.h>

class CompareNode : public Node
{
public:
    CompareOperatorType  compareOperator;
    CompareNode(Node* _left, Node* _right, CompareOperatorType _compareOperator)
            : Node(COMPARISON),  compareOperator(_compareOperator)
    {
        nodes.push_back(_right);
        nodes.push_back(_left);
    }

    llvm::Value* codegen(impala::Toolbox& tools) override {
      for (auto node: nodes) {
        node->codegen(tools);
      }
      std::cout << "CompareNode" << std::endl;
      return nullptr;
    }
};


#endif //IMPALAJIT_COMPARISON_EXPRESSION_H
