#include "ast/node.h"

#include <iostream>

ExpressionStatementNode::ExpressionStatementNode(ExpressionNode* node):
	content(node) {}

ExpressionStatementNode::~ExpressionStatementNode()
{
    delete this->content;
}

void ExpressionStatementNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "expression statement" << std::endl;
    this->content->print(os, level+1);
}

void ExpressionStatementNode::checkTypes(BrainfuckWriter& writer)
{
    this->content->checkTypes(writer);
}
