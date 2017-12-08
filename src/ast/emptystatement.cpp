#include "ast/node.h"

#include <iostream>

EmptyStatementNode::EmptyStatementNode() {}

void EmptyStatementNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "empty statement" << std::endl;
}
