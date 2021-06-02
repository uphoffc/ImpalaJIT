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

#ifndef IMPALAJIT_BASIC_EXPRESSION_H_HH
#define IMPALAJIT_BASIC_EXPRESSION_H_HH

#include <node.h>

class ConstantNode : public Node
{
public:
    double value;
    explicit ConstantNode(double _value)
            : Node(CONSTANT), value(_value)
    {
    }

    llvm::Value* codegen(impala::Toolbox& tools) override {
      assert(nodes.empty() && "ConstantNode must be a leaf node");
      std::cout << "ConstantNode" << std::endl;
      auto realType = llvm::Type::getDoubleTy(tools.context);
      return llvm::ConstantFP::get(realType, value);
    }
};

class VariableNode : public Node {

public:
    std::string &name;
    VariableNode(std::string &_name)
            : Node(VARIABLE), name(_name) {
    }

    llvm::Value* codegen(impala::Toolbox& tools) override {
      assert(nodes.size() == 0 && "VariableNode must have no children");
      std::cout << "VariableNode" << std::endl;
      if (tools.table.find(name) == tools.table.end()) {
        std::runtime_error("symbol `" + name + "` have not been found");
      }
      return tools.builder.CreateLoad(tools.table[name]);
    }
};

class NegationNode : public Node
{
public:
    NegationNode(Node* _node)
            : Node(NEGATION)
    {
        nodes.push_back(_node);
    }

    llvm::Value* codegen(impala::Toolbox& tools) override {
      assert(nodes.size() == 1 && "NegationNode must be a unary operation");
      auto expr = nodes[0]->codegen(tools);
      std::cout << "NegationNode" << std::endl;
      return tools.builder.CreateFNeg(expr);
    }
};

class AdditionNode : public Node
{
public:
    AdditionNode(Node* _left, Node* _right)
            : Node(ADDITION)
    {
        nodes.push_back(_left);
        nodes.push_back(_right);
    }

    llvm::Value* codegen(impala::Toolbox& tools) override {
      assert(nodes.size() == 2 && "AdditionNode must be a binary op");
      auto lhs = nodes[0]->codegen(tools);
      auto rhs = nodes[1]->codegen(tools);

      std::cout << "AdditionNode" << std::endl;
      return tools.builder.CreateFAdd(lhs, rhs);
    }
};

class SubtractionNode : public Node
{
public:
    SubtractionNode(Node* _left, Node* _right)
            : Node(SUBTRACTION)
    {
        nodes.push_back(_left);
        nodes.push_back(_right);
    }

    llvm::Value* codegen(impala::Toolbox& tools) override {
      assert(nodes.size() == 2 && "SubtractionNode must be a binary op");
      auto lhs = nodes[0]->codegen(tools);
      auto rhs = nodes[1]->codegen(tools);

      std::cout << "SubtractionNode" << std::endl;
      return tools.builder.CreateFSub(lhs, rhs);
    }
};

class MultiplicationNode : public Node
{
public:
    MultiplicationNode(Node* _left, Node* _right)
            : Node(MULTIPLICATION)
    {
        nodes.push_back(_left);
        nodes.push_back(_right);
    }

    llvm::Value* codegen(impala::Toolbox& tools) override {
      assert(nodes.size() == 2 && "MultiplicationNode must be a binary op");
      auto lhs = nodes[0]->codegen(tools);
      auto rhs = nodes[1]->codegen(tools);

      std::cout << "MultiplicationNode" << std::endl;
      return tools.builder.CreateFMul(lhs, rhs);
    }
};

class DivisionNode : public Node
{
public:
    DivisionNode(Node* &_left, Node* &_right)
            : Node(DIVISION)
    {
        nodes.push_back(_left);
        nodes.push_back(_right);
    }

    llvm::Value* codegen(impala::Toolbox& tools) override {
      assert(nodes.size() == 2 && "DivisionNode must be a binary op");
      auto lhs = nodes[0]->codegen(tools);
      auto rhs = nodes[1]->codegen(tools);

      std::cout << "DivisionNode" << std::endl;
      return tools.builder.CreateFDiv(lhs, rhs);
    }
};

class PowerNode : public Node
{
public:
    PowerNode(Node* _base, Node* _exponent)
            : Node(POWER)
    {
        nodes.push_back(_exponent);
        nodes.push_back(_base);
    }

    llvm::Value* codegen(impala::Toolbox& tools) override {
      assert(nodes.size() == 2 && "DivisionNode must be a binary op");
      auto lhs = nodes[0]->codegen(tools);
      auto rhs = nodes[1]->codegen(tools);

      std::cout << "PowerNode" << std::endl;
      return nullptr;
    }
};

#endif //IMPALAJIT_BASIC_EXPRESSION_H_HH
