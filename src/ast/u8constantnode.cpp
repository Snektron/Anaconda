#include "ast/node.h"

#include <iostream>

U8ConstantNode::U8ConstantNode(uint8_t value) : value(value) {}

U8ConstantNode::~U8ConstantNode() {}

void U8ConstantNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "u8 constant (" << (size_t)this->value << ")" << std::endl;
}
