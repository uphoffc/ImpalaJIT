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

#ifndef IMPALAJIT_COMPLEX_EXPRESSION_H
#define IMPALAJIT_COMPLEX_EXPRESSION_H

#include <node.h>

class IfStmtNode : public Node {
public:
  IfStmtNode(Node *_condition, Node *_if_body) : Node(IF_STATEMENT) {
    nodes.push_back(_condition);
    nodes.push_back(_if_body);
  }

  llvm::Value *codegen(impala::Toolbox &tools) override {
    assert(nodes.size() == 2 && "IfStmtNode must have two children nodes");
    auto cond = nodes[0]->codegen(tools);

    auto currFunction = tools.builder.GetInsertBlock()->getParent();
    auto thenBlock = llvm::BasicBlock::Create(tools.context);
    auto mergeBlock = llvm::BasicBlock::Create(tools.context);
    tools.builder.CreateCondBr(cond, thenBlock, mergeBlock);

    currFunction->getBasicBlockList().push_back(thenBlock);
    currFunction->getBasicBlockList().push_back(mergeBlock);

    tools.builder.SetInsertPoint(thenBlock);
    auto lastInstr = nodes[1]->codegen(tools);
    if (llvm::dyn_cast<llvm::ReturnInst>(lastInstr) != nullptr) {
      tools.builder.CreateBr(mergeBlock);
    }

    tools.builder.SetInsertPoint(mergeBlock);

    std::cout << "IfStmtNode" << std::endl;
    return nullptr;
  }
};

class IfElseStmtNode : public Node {
public:
  IfElseStmtNode(Node *_condition, Node *_if_body, Node *_else_body) : Node(IF_ELSE_STATEMENT) {
    nodes.push_back(_condition);
    nodes.push_back(_if_body);
    nodes.push_back(_else_body);
  }

  llvm::Value *codegen(impala::Toolbox &tools) override {
    assert(nodes.size() == 3 && "IfStmtNode must have three children nodes");
    auto cond = nodes[0]->codegen(tools);

    auto currFunction = tools.builder.GetInsertBlock()->getParent();
    auto thenBlock = llvm::BasicBlock::Create(tools.context);
    auto elseBlock = llvm::BasicBlock::Create(tools.context);
    auto mergeBlock = llvm::BasicBlock::Create(tools.context);

    currFunction->getBasicBlockList().push_back(thenBlock);
    currFunction->getBasicBlockList().push_back(elseBlock);

    tools.builder.CreateCondBr(cond, thenBlock, elseBlock);

    tools.builder.SetInsertPoint(thenBlock);
    auto lastThenBlockInstr = nodes[1]->codegen(tools);
    bool noReturnInThenBlock = llvm::dyn_cast<llvm::ReturnInst>(lastThenBlockInstr) == nullptr;
    if (noReturnInThenBlock) {
      tools.builder.CreateBr(mergeBlock);
    }

    tools.builder.SetInsertPoint(elseBlock);
    auto lastElseBlockInstr = nodes[2]->codegen(tools);
    bool noReturnInElseBlock = llvm::dyn_cast<llvm::ReturnInst>(lastElseBlockInstr) == nullptr;
    if (noReturnInElseBlock) {
      tools.builder.CreateBr(mergeBlock);
    }

    if (noReturnInThenBlock || noReturnInElseBlock) {
      currFunction->getBasicBlockList().push_back(mergeBlock);
      tools.builder.SetInsertPoint(mergeBlock);
    }

    std::cout << "IfElseStmtNode" << std::endl;
    return nullptr;
  }
};

class IfBodyNode : public Node {
public:
  IfBodyNode() : Node(IF_BODY) {}

  virtual ~IfBodyNode() {}

  llvm::Value *codegen(impala::Toolbox &tools) override {
    llvm::Value *lastInstruction{nullptr};
    for (auto node : nodes) {
      lastInstruction = node->codegen(tools);
      if (llvm::dyn_cast<llvm::ReturnInst>(lastInstruction)) {
        // a return instruction found. Doesn't make sense to generate the rest of the instructions
        break;
      }
    }
    std::cout << "IfBodyNode" << std::endl;
    return lastInstruction;
  }
};

class ElseBodyNode : public Node {
public:
  ElseBodyNode() : Node(ELSE_BODY) {}

  virtual ~ElseBodyNode() {}

  llvm::Value *codegen(impala::Toolbox &tools) override {
    llvm::Value *lastInstruction{nullptr};
    for (auto node : nodes) {
      lastInstruction = node->codegen(tools);
      if (llvm::dyn_cast<llvm::ReturnInst>(lastInstruction)) {
        // a return instruction found. Doesn't make sense to generate the rest of the instructions
        break;
      }
    }
    std::cout << "ElseBodyNode" << std::endl;
    return lastInstruction;
  }
};
#endif // IMPALAJIT_COMPLEX_EXPRESSION_H
