#ifndef IMPALA_CPP_PRETTY_PRINTER_H
#define IMPALA_CPP_PRETTY_PRINTER_H

#include "abstract_visitor.h"
#include <string>

namespace impala {
class PrettyPrinter : public AbstractVisitor {
public:
  void visit(RootNode *node) override;
  void visit(ReturnNode *node) override;
  void visit(ExternalFunctionNode *node) override;
  void visit(ExternalFunctionParametersNode *node) override;
  void visit(PowerNode *node) override;
  void visit(DivisionNode *node) override;
  void visit(MultiplicationNode *node) override;
  void visit(SubtractionNode *node) override;
  void visit(AdditionNode *node) override;
  void visit(NegationNode *node) override;
  void visit(VariableNode *node) override;
  void visit(ConstantNode *node) override;
  void visit(CompareNode *node) override;
  void visit(BooleanAndNode *node) override;
  void visit(BooleanOrNode *node) override;
  void visit(AssignmentNode *node) override;
  void visit(IfStmtNode *node) override;
  void visit(IfElseStmtNode *node) override;
  void visit(IfBodyNode *node) override;
  void visit(ElseBodyNode *node) override;

private:
  static constexpr int IndentSize{2};
  static constexpr char IndentChar{'-'};
  void increment() { indent += IndentSize; }
  void decrement() { indent -= IndentSize; }
  std::string getIndentLine() { return std::string(indent, IndentChar) + "> "; }

  int indent{0};
};
} // namespace impala

#endif // IMPALA_CPP_PRETTY_PRINTER_H
