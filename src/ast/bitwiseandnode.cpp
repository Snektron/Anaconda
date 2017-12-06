#include "ast/node.h"

#include <iostream>

BitwiseAndNode::BitwiseAndNode(ExpressionNode* lop, ExpressionNode* rop):
	BinaryExpressionNode(lop, rop) {}

void BitwiseAndNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "bitwise and expression" << std::endl;
    this->lop->print(os, level+1);
    this->rop->print(os, level+1);
}
