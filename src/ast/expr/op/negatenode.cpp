#include "ast/expr/op/negatenode.h"
#include "generator/brainfuck.h"

#include <iostream>

NegateNode::NegateNode(ExpressionNode* op):
    UnaryOperatorNode(op) {}

void NegateNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "negation expression";
    this->op->print(os, level+1);
}

void NegateNode::generate(BrainfuckWriter& writer)
{
    ///TODO
    writer.unimplemented();
}
