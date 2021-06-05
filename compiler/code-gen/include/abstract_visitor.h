#ifndef IMPALA_CPP_ABSTRACTVISITOR_H
#define IMPALA_CPP_ABSTRACTVISITOR_H

class RootNode;
class ReturnNode;
class ExternalFunctionNode;
class ExternalFunctionParametersNode;
class PowerNode;
class DivisionNode;
class MultiplicationNode;
class SubtractionNode;
class AdditionNode;
class NegationNode;
class VariableNode;
class ConstantNode;
class ElseBodyNode;
class IfBodyNode;
class IfElseStmtNode;
class IfStmtNode;
class CompareNode;
class BooleanAndNode;
class BooleanOrNode;
class AssignmentNode;

namespace impala {
  class AbstractVisitor {
  public:
    virtual ~AbstractVisitor() = default;
    virtual void visit(RootNode* node) = 0;
    virtual void visit(ReturnNode* node) = 0;
    virtual void visit(ExternalFunctionNode* node) = 0;
    virtual void visit(ExternalFunctionParametersNode* node) = 0;
    virtual void visit(PowerNode* node) = 0;
    virtual void visit(DivisionNode* node) = 0;
    virtual void visit(MultiplicationNode* node) = 0;
    virtual void visit(SubtractionNode* node) = 0;
    virtual void visit(AdditionNode* node) = 0;
    virtual void visit(NegationNode* node) = 0;
    virtual void visit(VariableNode* node) = 0;
    virtual void visit(ConstantNode* node) = 0;
    virtual void visit(ElseBodyNode* node) = 0;
    virtual void visit(IfBodyNode* node) = 0;
    virtual void visit(IfElseStmtNode* node) = 0;
    virtual void visit(IfStmtNode* node) = 0;
    virtual void visit(CompareNode* node) = 0;
    virtual void visit(BooleanAndNode* node) = 0;
    virtual void visit(BooleanOrNode* node) = 0;
    virtual void visit(AssignmentNode* node) = 0;
  };
}

#endif //IMPALA_CPP_ABSTRACTVISITOR_H
