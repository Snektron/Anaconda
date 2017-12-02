#include "ast/node.h"

#include <iostream>

BitwiseRightShiftNode::BitwiseRightShiftNode(ExpressionNode* lop, ExpressionNode* rop) : lop(lop), rop(rop) {}

BitwiseRightShiftNode::~BitwiseRightShiftNode()
{
    delete this->lop;
    delete this->rop;
}

void BitwiseRightShiftNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "bitwise right shift expression" << std::endl;
    this->lop->print(os, level+1);
    this->rop->print(os, level+1);
}
