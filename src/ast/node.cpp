#include "ast/node.h"

#include <iostream>

const char* NODE_PRINT_INDENT = "    ";

void Node::printIndent(std::ostream& output, size_t level) const
{
    for(size_t i = 0; i < level; ++i)
        output << NODE_PRINT_INDENT;
}

void Node::declareGlobals(BrainfuckWriter& writer) const {}
