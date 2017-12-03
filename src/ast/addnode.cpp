#include "ast/node.h"

#include <iostream>

AddNode::AddNode(ExpressionNode* lop, ExpressionNode* rop):
	lop(lop), rop(rop) {}

AddNode::~AddNode()
{
    delete this->lop;
    delete this->rop;
}

void AddNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "addition expression" << std::endl;
    this->lop->print(os, level+1);
    this->rop->print(os, level+1);
}
