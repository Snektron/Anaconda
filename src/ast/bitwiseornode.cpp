#include "ast/node.h"
#include "generator/brainfuck.h"

#include <iostream>

BitwiseOrNode::BitwiseOrNode(ExpressionNode* lop, ExpressionNode* rop):
    BinaryExpressionNode(lop, rop) {}

void BitwiseOrNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "bitwise or expression" << std::endl;
    this->lop->print(os, level+1);
    this->rop->print(os, level+1);
}

void BitwiseOrNode::generate(BrainfuckWriter& writer)
{
    ///TODO
    writer.unimplemented();
}
