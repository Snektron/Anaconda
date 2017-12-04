#include "ast/node.h"

#include <iostream>

BlockNode::BlockNode(StatementNode* content):
	content(content) {}

BlockNode::~BlockNode()
{
    delete this->content;
}

void BlockNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "block node" << std::endl;
    this->content->print(os, level+1);
}

void BlockNode::checkTypes(BrainfuckWriter& writer)
{
    this->content->checkTypes(writer);
}
