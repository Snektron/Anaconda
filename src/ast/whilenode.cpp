#include "ast/node.h"

#include <iostream>

WhileNode::WhileNode(ExpressionNode* conditional, StatementNode* statement):
	conditional(conditional), statement(statement) {}

WhileNode::~WhileNode()
{
    delete this->conditional;
    delete this->statement;
}

void WhileNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "while statement" << std::endl;
    this->conditional->print(os, level+1);
    this->statement->print(os, level+1);
}
