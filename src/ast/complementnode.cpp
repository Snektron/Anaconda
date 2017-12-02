#include "ast/node.h"

#include <iostream>

ComplementNode::ComplementNode(ExpressionNode* op) : op(op) {}

ComplementNode::~ComplementNode()
{
    delete this->op;
}

void ComplementNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "bitwise complement expression";
    this->op->print(os, level+1);
}