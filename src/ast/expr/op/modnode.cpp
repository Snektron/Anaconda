#include "ast/expr/op/modnode.h"
#include "generator/brainfuck.h"

#include <iostream>

ModNode::ModNode(ExpressionNode* lop, ExpressionNode* rop):
    BinaryExpressionNode(lop, rop) {}

void ModNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "modulo expression" << std::endl;
    this->lop->print(os, level+1);
    this->rop->print(os, level+1);
}

void ModNode::generate(BrainfuckWriter& writer)
{
    ///TODO
    writer.unimplemented();
}
