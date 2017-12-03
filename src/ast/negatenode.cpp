#include "ast/node.h"

#include <iostream>

NegateNode::NegateNode(ExpressionNode* op):
	op(op) {}

NegateNode::~NegateNode()
{
    delete this->op;
}

void NegateNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "negation expression";
    this->op->print(os, level+1);
}
