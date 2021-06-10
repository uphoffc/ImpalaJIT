#include "codegen_visitor.h"
#include "assignment_nodes.h"
#include "boolean_nodes.h"
#include "compare_nodes.h"
#include "conditional_nodes.h"
#include "expression_nodes.h"
#include "external_function_nodes.h"
#include "return_nodes.h"

namespace impala {
void CodegenVisitor::visit(RootNode *node) {
  for (auto statements : node->nodes) {
    statements->accept(this);
  }
};

void CodegenVisitor::visit(ReturnNode *node) {
  assert(node->nodes.size() == 1 && "ReturnNode must have one child");
  node->nodes[0]->accept(this);
  auto returnValue = top();
  stack.push(toolbox.builder->CreateRet(returnValue));
}

void CodegenVisitor::visit(ExternalFunctionNode *node) {
  std::string calleeName = node->name;
  std::stringstream errStream;
  if (toolbox.externalMathFunctions.find(calleeName) == toolbox.externalMathFunctions.end()) {
    errStream << "impala: function `" << calleeName << "` doesn't belong to the standard math library";
    throw std::runtime_error(errStream.str());
  }

  auto proto = toolbox.externalMathFunctions[calleeName];
  if (node->nodes.size() != proto->arg_size()) {
    errStream << "impala: function `" << calleeName << "` takes " << proto->arg_size() << " parameters, "
              << "given " << node->nodes.size();
    throw std::runtime_error(errStream.str());
  }

  std::vector<llvm::Value *> args;
  for (auto child : node->nodes) {
    child->accept(this);
    if (stack.top()) {
      args.push_back(top());
    } else {
      errStream << "impala: found a statement in a list of formal parameters. "
                << "Please, check `" << calleeName << "` function call";
      throw std::runtime_error(errStream.str());
    }
  }
  stack.push(toolbox.builder->CreateCall(proto, args));
}

void CodegenVisitor::visit(ExternalFunctionParametersNode *node) {
  // TODO: delete
  throw std::runtime_error("impala: ExternalFunctionParametersNode has never been supported by the language");
}

void CodegenVisitor::visit(PowerNode *node) {
  // TODO: delete
  assert(node->nodes.size() == 2 && "PowerNode must be a binary op");
  throw std::runtime_error("impala: PowerNode has never been supported by the language");
}

void CodegenVisitor::visit(DivisionNode *node) {
  assert(node->nodes.size() == 2 && "DivisionNode must be a binary op");
  node->nodes[0]->accept(this);
  auto lhs = top();
  node->nodes[1]->accept(this);
  auto rhs = top();
  stack.push(toolbox.builder->CreateFDiv(lhs, rhs));
}

void CodegenVisitor::visit(MultiplicationNode *node) {
  assert(node->nodes.size() == 2 && "MultiplicationNode must be a binary op");
  node->nodes[0]->accept(this);
  auto lhs = top();
  node->nodes[1]->accept(this);
  auto rhs = top();
  stack.push(toolbox.builder->CreateFMul(lhs, rhs));
}

void CodegenVisitor::visit(SubtractionNode *node) {
  assert(node->nodes.size() == 2 && "SubtractionNode must be a binary op");
  node->nodes[0]->accept(this);
  auto lhs = top();
  node->nodes[1]->accept(this);
  auto rhs = top();
  stack.push(toolbox.builder->CreateFSub(lhs, rhs));
}

void CodegenVisitor::visit(AdditionNode *node) {
  assert(node->nodes.size() == 2 && "AdditionNode must be a binary op");
  node->nodes[0]->accept(this);
  auto lhs = top();
  node->nodes[1]->accept(this);
  auto rhs = top();
  stack.push(toolbox.builder->CreateFAdd(lhs, rhs));
}

void CodegenVisitor::visit(NegationNode *node) {
  assert(node->nodes.size() == 1 && "NegationNode must be a binary op");
  node->nodes[0]->accept(this);
  auto expr = top();
  stack.push(toolbox.builder->CreateFNeg(expr));
}

void CodegenVisitor::visit(VariableNode *node) {
  assert(node->nodes.empty() && "VariableNode must have no children");
  auto address = toolbox.symbolTable[node->name];
  if (!address) {
    throw std::runtime_error("symbol `" + node->name + "` have not been defined");
  }
  stack.push(toolbox.builder->CreateLoad(address));
}

void CodegenVisitor::visit(ConstantNode *node) {
  assert(node->nodes.empty() && "ConstantNode must have no children");
  stack.push(llvm::ConstantFP::get(realType, node->value));
}

void CodegenVisitor::visit(CompareNode *node) {
  assert(node->nodes.size() == 2 && "CompareNode must be a binary op");
  // NOTE: it was originally mixed up
  node->nodes[1]->accept(this);
  auto lhs = top();
  node->nodes[0]->accept(this);
  auto rhs = top();

  llvm::Value *cmpResult{nullptr};
  switch (node->compareOperator) {
  case CompareOperatorType::EQ: {
    cmpResult = toolbox.builder->CreateFCmpOEQ(lhs, rhs);
    break;
  }
  case CompareOperatorType::NE: {
    cmpResult = toolbox.builder->CreateFCmpONE(lhs, rhs);
    break;
  }
  case CompareOperatorType::GT: {
    cmpResult = toolbox.builder->CreateFCmpOGT(lhs, rhs);
    break;
  }
  case CompareOperatorType::LT: {
    cmpResult = toolbox.builder->CreateFCmpOLT(lhs, rhs);
    break;
  }
  case CompareOperatorType::GTE: {
    cmpResult = toolbox.builder->CreateFCmpOGE(lhs, rhs);
    break;
  }
  case CompareOperatorType::LTE: {
    cmpResult = toolbox.builder->CreateFCmpOLE(lhs, rhs);
    break;
  }
  default: {
    std::string msg = "Unknown comparison operator: " + std::to_string(static_cast<char>(node->compareOperator));
    throw std::runtime_error(msg);
  }
  }
  stack.push(cmpResult);
}

void CodegenVisitor::visit(BooleanAndNode *node) {
  assert(node->nodes.size() == 2 && "BooleanAndNode must be a binary op");
  node->nodes[0]->accept(this);
  auto lhs = top();
  node->nodes[1]->accept(this);
  auto rhs = top();

  stack.push(toolbox.builder->CreateAnd(lhs, rhs));
}

void CodegenVisitor::visit(BooleanOrNode *node) {
  assert(node->nodes.size() == 2 && "BooleanOrNode must be a binary op");
  node->nodes[0]->accept(this);
  auto lhs = top();
  node->nodes[1]->accept(this);
  auto rhs = top();

  stack.push(toolbox.builder->CreateOr(lhs, rhs));
}

void CodegenVisitor::visit(AssignmentNode *node) {
  assert(node->nodes.size() == 1 && "AssignmentNode must have one child expr");
  node->nodes[0]->accept(this);
  auto expr = top();

  auto address = toolbox.symbolTable[node->name];
  if (!address) {
    address = toolbox.builder->CreateAlloca(realType);
    toolbox.symbolTable.addSymbol(node->name, address);
  }
  toolbox.builder->CreateStore(expr, address);
}

void CodegenVisitor::visit(IfStmtNode *node) {
  assert(node->nodes.size() == 2 && "IfStmtNode must have three children nodes");
  node->nodes[0]->accept(this);
  auto predicate = top();

  auto currFunction = toolbox.builder->GetInsertBlock()->getParent();
  auto thenBlock = llvm::BasicBlock::Create(toolbox.context);
  auto mergeBlock = llvm::BasicBlock::Create(toolbox.context);
  toolbox.builder->CreateCondBr(predicate, thenBlock, mergeBlock);

  currFunction->getBasicBlockList().push_back(thenBlock);
  currFunction->getBasicBlockList().push_back(mergeBlock);

  toolbox.builder->SetInsertPoint(thenBlock);
  node->nodes[1]->accept(this);
  auto &lastInstr = toolbox.builder->GetInsertBlock()->back();
  if (!llvm::isa<llvm::ReturnInst>(lastInstr)) {
    toolbox.builder->CreateBr(mergeBlock);
  }

  toolbox.builder->SetInsertPoint(mergeBlock);
}

void CodegenVisitor::visit(IfElseStmtNode *node) {
  assert(node->nodes.size() == 3 && "IfStmtNode must have three children nodes");
  node->nodes[0]->accept(this);
  auto predicate = top();

  auto currFunction = toolbox.builder->GetInsertBlock()->getParent();
  auto thenBlock = llvm::BasicBlock::Create(toolbox.context);
  auto elseBlock = llvm::BasicBlock::Create(toolbox.context);
  auto mergeBlock = llvm::BasicBlock::Create(toolbox.context);

  currFunction->getBasicBlockList().push_back(thenBlock);
  currFunction->getBasicBlockList().push_back(elseBlock);

  toolbox.builder->CreateCondBr(predicate, thenBlock, elseBlock);

  toolbox.builder->SetInsertPoint(thenBlock);
  node->nodes[1]->accept(this);
  auto &lastThenBlockInstr = toolbox.builder->GetInsertBlock()->back();
  bool noReturnInThenBlock = !llvm::isa<llvm::ReturnInst>(lastThenBlockInstr);
  if (noReturnInThenBlock) {
    toolbox.builder->CreateBr(mergeBlock);
  }

  toolbox.builder->SetInsertPoint(elseBlock);
  node->nodes[2]->accept(this);
  auto &lastElseBlockInstr = toolbox.builder->GetInsertBlock()->back();
  bool noReturnInElseBlock = !llvm::isa<llvm::ReturnInst>(lastElseBlockInstr);
  if (noReturnInElseBlock) {
    toolbox.builder->CreateBr(mergeBlock);
  }

  if (noReturnInThenBlock || noReturnInElseBlock) {
    currFunction->getBasicBlockList().push_back(mergeBlock);
    toolbox.builder->SetInsertPoint(mergeBlock);
  }
}

void CodegenVisitor::visit(IfBodyNode *node) {
  toolbox.symbolTable.addScope();
  for (auto child : node->nodes) {
    child->accept(this);
    auto &lastInstruction = toolbox.builder->GetInsertBlock()->back();
    if (llvm::isa<llvm::ReturnInst>(lastInstruction)) {
      // a return instruction found. Doesn't make sense to generate the rest of the instructions
      break;
    }
  }
  toolbox.symbolTable.removeScope();
}

void CodegenVisitor::visit(ElseBodyNode *node) {
  toolbox.symbolTable.addScope();
  for (auto child : node->nodes) {
    child->accept(this);
    auto &lastInstruction = toolbox.builder->GetInsertBlock()->back();
    if (llvm::isa<llvm::ReturnInst>(lastInstruction)) {
      // a return instruction found. Doesn't make sense to generate the rest of the instructions
      break;
    }
  }
  toolbox.symbolTable.removeScope();
}

} // namespace impala