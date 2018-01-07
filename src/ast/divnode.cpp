#include "ast/node.h"
#include "generator/brainfuck.h"

#include <iostream>

DivNode::DivNode(ExpressionNode* lop, ExpressionNode* rop):
    BinaryExpressionNode(lop, rop) {}

void DivNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "division expression" << std::endl;
    this->lop->print(os, level+1);
    this->rop->print(os, level+1);
}

void DivNode::generate(BrainfuckWriter& writer)
{
    ///TODO
    writer.unimplemented();
}
