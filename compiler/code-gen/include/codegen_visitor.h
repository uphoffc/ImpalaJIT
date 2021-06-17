#ifndef IMPALA_CPP_CODEGENVISITOR_H
#define IMPALA_CPP_CODEGENVISITOR_H

#include "abstract_visitor.h"
#include "engine.h"
#include <stack>

namespace impala {
class CodegenVisitor : public AbstractVisitor {
public:
  CodegenVisitor(engine::Jit::Toolbox &tools, llvm::Type *realType) : toolbox(tools), realType(realType) {}
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
  llvm::Value *top() {
    auto topValue = stack.top();
    stack.pop();
    return topValue;
  }

  std::stack<llvm::Value *> stack{};
  engine::Jit::Toolbox &toolbox;
  llvm::Type *realType{nullptr};
};
} // namespace impala

#endif // IMPALA_CPP_CODEGENVISITOR_H
