#include "ast/node.h"

#include <iostream>

MulNode::MulNode(ExpressionNode* lop, ExpressionNode* rop):
    BinaryExpressionNode(lop, rop) {}

void MulNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "multiplication expression" << std::endl;
    this->lop->print(os, level+1);
    this->rop->print(os, level+1);
}
