#include "ast/expr/op/complementnode.h"
#include "generator/brainfuck.h"

#include <iostream>

ComplementNode::ComplementNode(ExpressionNode* op):
    UnaryExpressionNode(op) {}

void ComplementNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "bitwise complement expression";
    this->op->print(os, level+1);
}

void ComplementNode::generate(BrainfuckWriter& writer)
{
    ///TODO
    writer.unimplemented();
}
