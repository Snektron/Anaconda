#include "ast/expr/op/subnode.h"
#include "generator/brainfuck.h"

#include <iostream>

SubNode::SubNode(ExpressionNode* lop, ExpressionNode* rop):
    BinaryExpressionNode(lop, rop) {}

void SubNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "subtraction expression" << std::endl;
    this->lop->print(os, level+1);
    this->rop->print(os, level+1);
}

void SubNode::generate(BrainfuckWriter& writer)
{
    if(this->type->equals(DataType<DataTypeClass::U8>()))
        writer.subU8();
    else
        writer.unimplemented();
}
