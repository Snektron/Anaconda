#include "ast/node.h"

#include <iostream>

SubNode::SubNode(ExpressionNode* lop, ExpressionNode* rop):
	lop(lop), rop(rop) {}

SubNode::~SubNode()
{
    delete this->lop;
    delete this->rop;
}

void SubNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "subtraction expression" << std::endl;
    this->lop->print(os, level+1);
    this->rop->print(os, level+1);
}
