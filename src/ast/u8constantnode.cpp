#include "ast/node.h"
#include "generator/brainfuck.h"
#include "util/utils.h"

#include <iostream>

U8ConstantNode::U8ConstantNode(uint8_t value):
    value(value) {}

U8ConstantNode::~U8ConstantNode() {}

void U8ConstantNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "u8 constant (" << (size_t)this->value << ")" << std::endl;
}

void U8ConstantNode::generate(BrainfuckWriter& writer)
{
    writer.pushByte(this->value);
}

void U8ConstantNode::checkTypes(BrainfuckWriter& writer)
{
    UNUSED(writer);
}

DataTypeBase* U8ConstantNode::getType()
{
    return new DataType<DataTypeClass::U8>();
}

void U8ConstantNode::declareLocals(BrainfuckWriter& writer)
{
    UNUSED(writer);
}
