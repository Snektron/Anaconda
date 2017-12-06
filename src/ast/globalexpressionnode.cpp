#include "ast/node.h"
#include "types/datatype.h"
#include "generator/brainfuck.h"
#include "except/exceptions.h"

#include <iostream>
#include <memory>
#include <sstream>

GlobalExpressionNode::GlobalExpressionNode(ExpressionNode* expression) : expression(expression) {}
GlobalExpressionNode::~GlobalExpressionNode()
{
    delete this->expression;
}

void GlobalExpressionNode::print(std::ostream& os, size_t size) const
{
    this->printIndent(os, size);
    os << "global expression" << std::endl;
    this->expression->print(os, size+1);
}

void GlobalExpressionNode::declareGlobals(BrainfuckWriter& writer)
{
    this->expression->declareGlobals(writer);
}

void GlobalExpressionNode::checkTypes(BrainfuckWriter& writer)
{
    this->expression->checkTypes(writer);
}
