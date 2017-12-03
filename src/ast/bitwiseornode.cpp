#include "ast/node.h"

#include <iostream>

BitwiseOrNode::BitwiseOrNode(ExpressionNode* lop, ExpressionNode* rop):
	lop(lop), rop(rop) {}

BitwiseOrNode::~BitwiseOrNode()
{
    delete this->lop;
    delete this->rop;
}

void BitwiseOrNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "bitwise or expression" << std::endl;
    this->lop->print(os, level+1);
    this->rop->print(os, level+1);
}
