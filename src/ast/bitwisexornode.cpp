#include "ast/node.h"

#include <iostream>

BitwiseXorNode::BitwiseXorNode(ExpressionNode* lop, ExpressionNode* rop):
	lop(lop), rop(rop) {}

BitwiseXorNode::~BitwiseXorNode()
{
    delete this->lop;
    delete this->rop;
}

void BitwiseXorNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "bitwise xor expression" << std::endl;
    this->lop->print(os, level+1);
    this->rop->print(os, level+1);
}
