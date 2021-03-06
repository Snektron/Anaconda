#include "ast/expr/op/bitwisexornode.h"
#include "generator/brainfuck.h"

#include <iostream>

BitwiseXorNode::BitwiseXorNode(ExpressionNode* lop, ExpressionNode* rop):
    BinaryOperatorNode(lop, rop) {}

void BitwiseXorNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "bitwise xor expression" << std::endl;
    this->lop->print(os, level+1);
    this->rop->print(os, level+1);
}

void BitwiseXorNode::generate(BrainfuckWriter& writer)
{
    ///TODO
    writer.unimplemented();
}
