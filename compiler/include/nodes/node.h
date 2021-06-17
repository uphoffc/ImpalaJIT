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

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "engine.h"
#include "abstract_visitor.h"
#include <internal_types.hh>
#include <iostream>
#include <vector>


class Node {
public:
  NodeType nodeType;
  std::vector<Node *> nodes;

  Node(NodeType _nodeType) : nodeType(_nodeType) {}
  virtual ~Node() { nodes.clear(); }

  virtual void accept(impala::AbstractVisitor* visitor) = 0;
};

class RootNode : public Node {
public:
  RootNode() : Node(ROOT) {}

  void accept(impala::AbstractVisitor* visitor) override {
    visitor->visit(this);
  }
};

#endif // EXPRESSION_H
