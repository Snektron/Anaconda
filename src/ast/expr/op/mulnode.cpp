#include "ast/expr/op/mulnode.h"
#include "generator/brainfuck.h"

#include <iostream>

MulNode::MulNode(ExpressionNode* lop, ExpressionNode* rop):
    BinaryOperatorNode(lop, rop) {}

void MulNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "multiplication expression" << std::endl;
    this->lop->print(os, level+1);
    this->rop->print(os, level+1);
}

void MulNode::generate(BrainfuckWriter& writer)
{
    if(this->type->equals(DataType<DataTypeClass::U8>()))
        writer.mulU8();
    else
        writer.unimplemented();
}
