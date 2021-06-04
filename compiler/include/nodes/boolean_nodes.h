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

#ifndef IMPALAJIT_BOOL_H
#define IMPALAJIT_BOOL_H

#include <node.h>

class BooleanAndNode : public Node {

public:
  BooleanAndNode() : Node(BOOLEAN_AND_JUNCTION) {}

  llvm::Value *codegen(impala::engine::Jit::Toolbox &tools) override {
    assert(nodes.size() == 2 && "BooleanAndNode must be a binary op");
    auto lhs = nodes[0]->codegen(tools);
    auto rhs = nodes[1]->codegen(tools);

    std::cout << "BooleanAndNode" << std::endl;
    return tools.builder->CreateAnd(lhs, rhs);
  }
};

class BooleanOrNode : public Node {

public:
  BooleanOrNode() : Node(BOOLEAN_OR_JUNCTION) {}

  llvm::Value *codegen(impala::engine::Jit::Toolbox &tools) override {
    assert(nodes.size() == 2 && "BooleanOrNode must be a binary op");
    auto lhs = nodes[0]->codegen(tools);
    auto rhs = nodes[1]->codegen(tools);

    std::cout << "BooleanOrNode" << std::endl;
    return tools.builder->CreateOr(lhs, rhs);
  }
};

#endif // IMPALAJIT_BOOL_H
