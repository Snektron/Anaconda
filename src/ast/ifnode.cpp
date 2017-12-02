#include "ast/node.h"

#include <iostream>

IfNode::IfNode(ExpressionNode* conditional, StatementNode* statement) : conditional(conditional), statement(statement) {}

IfNode::~IfNode()
{
    delete this->conditional;
    delete this->statement;
}

void IfNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "if statement" << std::endl;
    this->conditional->print(os, level+1);
    this->statement->print(os, level+1);
}
