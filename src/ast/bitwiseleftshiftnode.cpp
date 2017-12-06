#include "ast/node.h"

#include <iostream>

BitwiseLeftShiftNode::BitwiseLeftShiftNode(ExpressionNode* lop, ExpressionNode* rop):
	BinaryExpressionNode(lop, rop) {}

void BitwiseLeftShiftNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "bitwise left shift expression" << std::endl;
    this->lop->print(os, level+1);
    this->rop->print(os, level+1);
}
