#include "pretty_printer.h"
#include "assignment_nodes.h"
#include "boolean_nodes.h"
#include "compare_nodes.h"
#include "conditional_nodes.h"
#include "expression_nodes.h"
#include "external_function_nodes.h"
#include "return_nodes.h"

namespace impala {
void PrettyPrinter::visit(RootNode *node) {
  std::cout << getIndentLine() << "root" << std::endl;

  increment();
  for (auto statements : node->nodes) {
    statements->accept(this);
  }
  decrement();
};

void PrettyPrinter::visit(ReturnNode *node) {
  assert(node->nodes.size() == 1 && "ReturnNode must have one child");
  std::cout << getIndentLine() << "return" << std::endl;

  increment();
  node->nodes[0]->accept(this);
  decrement();
}

void PrettyPrinter::visit(ExternalFunctionNode *node) {
  std::cout << getIndentLine() << "extern call: " << node->name << std::endl;

  increment();
  for (auto child : node->nodes) {
    child->accept(this);
  }
  decrement();
}

void PrettyPrinter::visit(ExternalFunctionParametersNode *node) {
  std::cout << getIndentLine() << "call" << std::endl;

  increment();
  for (auto child : node->nodes) {
    child->accept(this);
  }
  decrement();
}

void PrettyPrinter::visit(PowerNode *node) {
  assert(node->nodes.size() == 2 && "PowerNode must be a binary op");
  std::cout << getIndentLine() << "pow" << std::endl;

  increment();
  node->nodes[0]->accept(this);
  node->nodes[1]->accept(this);
  decrement();
}

void PrettyPrinter::visit(DivisionNode *node) {
  assert(node->nodes.size() == 2 && "DivisionNode must be a binary op");
  std::cout << getIndentLine() << "/" << std::endl;

  increment();
  node->nodes[0]->accept(this);
  node->nodes[1]->accept(this);
  decrement();
}

void PrettyPrinter::visit(MultiplicationNode *node) {
  assert(node->nodes.size() == 2 && "MultiplicationNode must be a binary op");
  std::cout << getIndentLine() << "*" << std::endl;

  increment();
  node->nodes[0]->accept(this);
  node->nodes[1]->accept(this);
  decrement();
}

void PrettyPrinter::visit(SubtractionNode *node) {
  assert(node->nodes.size() == 2 && "SubtractionNode must be a binary op");
  std::cout << getIndentLine() << "-" << std::endl;

  increment();
  node->nodes[0]->accept(this);
  node->nodes[1]->accept(this);
  decrement();
}

void PrettyPrinter::visit(AdditionNode *node) {
  assert(node->nodes.size() == 2 && "AdditionNode must be a binary op");
  std::cout << getIndentLine() << "+" << std::endl;

  increment();
  node->nodes[0]->accept(this);
  node->nodes[1]->accept(this);
  decrement();
}

void PrettyPrinter::visit(NegationNode *node) {
  assert(node->nodes.size() == 1 && "NegationNode must be a binary op");
  std::cout << getIndentLine() << "NOR" << std::endl;

  increment();
  node->nodes[0]->accept(this);
  decrement();
}

void PrettyPrinter::visit(VariableNode *node) {
  assert(node->nodes.empty() && "VariableNode must have no children");
  std::cout << getIndentLine() << node->name << std::endl;
}

void PrettyPrinter::visit(ConstantNode *node) {
  assert(node->nodes.empty() && "ConstantNode must have no children");
  std::cout << getIndentLine() << node->value << std::endl;
}

void PrettyPrinter::visit(CompareNode *node) {
  assert(node->nodes.size() == 2 && "CompareNode must be a binary op");
  std::cout << getIndentLine() << "COMP" << std::endl;

  increment();
  node->nodes[0]->accept(this);
  node->nodes[1]->accept(this);
  decrement();
}

void PrettyPrinter::visit(BooleanAndNode *node) {
  assert(node->nodes.size() == 2 && "BooleanAndNode must be a binary op");
  std::cout << getIndentLine() << "AND" << std::endl;

  increment();
  node->nodes[0]->accept(this);
  node->nodes[1]->accept(this);
  decrement();
}

void PrettyPrinter::visit(BooleanOrNode *node) {
  assert(node->nodes.size() == 2 && "BooleanOrNode must be a binary op");
  std::cout << getIndentLine() << "OR" << std::endl;

  increment();
  node->nodes[0]->accept(this);
  node->nodes[1]->accept(this);
  decrement();
}

void PrettyPrinter::visit(AssignmentNode *node) {
  assert(node->nodes.size() == 1 && "AssignmentNode must have one child expr");

  std::cout << getIndentLine() << node->name << " :=" << std::endl;
  increment();
  node->nodes[0]->accept(this);
  decrement();
}

void PrettyPrinter::visit(IfStmtNode *node) {
  assert(node->nodes.size() == 2 && "IfStmtNode must have three children nodes");
  std::cout << getIndentLine() << "if" << std::endl;
  increment();
  node->nodes[0]->accept(this);
  node->nodes[1]->accept(this);
  decrement();
}

void PrettyPrinter::visit(IfElseStmtNode *node) {
  assert(node->nodes.size() == 3 && "IfStmtNode must have three children nodes");
  std::cout << getIndentLine() << "if" << std::endl;

  increment();
  node->nodes[0]->accept(this);
  node->nodes[1]->accept(this);
  node->nodes[2]->accept(this);
  decrement();
}

void PrettyPrinter::visit(IfBodyNode *node) {
  std::cout << getIndentLine() << "then" << std::endl;
  increment();
  for (auto stmt : node->nodes) {
    stmt->accept(this);
  }
  decrement();
}

void PrettyPrinter::visit(ElseBodyNode *node) {
  std::cout << getIndentLine() << "else" << std::endl;
  increment();
  for (auto stmt : node->nodes) {
    stmt->accept(this);
  }
  decrement();
}
} // namespace impala