#include "ast/node.h"
#include "common/util.h"

#include <iostream>

EmptyStatementNode::EmptyStatementNode() {}

void EmptyStatementNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "empty statement" << std::endl;
}

void EmptyStatementNode::checkTypes(BrainfuckWriter& writer)
{
    UNUSED(writer);
}

void EmptyStatementNode::generate(BrainfuckWriter& writer)
{
    UNUSED(writer);
}

void EmptyStatementNode::declareLocals(BrainfuckWriter& writer)
{
    UNUSED(writer);
}
