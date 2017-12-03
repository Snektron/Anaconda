#include "ast/node.h"

#include <iostream>

DivNode::DivNode(ExpressionNode* lop, ExpressionNode* rop):
	lop(lop), rop(rop) {}

DivNode::~DivNode()
{
    delete this->lop;
    delete this->rop;
}

void DivNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "division expression" << std::endl;
    this->lop->print(os, level+1);
    this->rop->print(os, level+1);
}
