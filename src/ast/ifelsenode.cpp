#include "ast/node.h"

#include <iostream>

IfElseNode::IfElseNode(ExpressionNode* conditional, StatementNode* statement, StatementNode* else_statement):
	conditional(conditional), statement(statement), else_statement(else_statement) {}

IfElseNode::~IfElseNode()
{
    delete this->conditional;
    delete this->statement;
    delete this->else_statement;
}

void IfElseNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "if-else statement" << std::endl;
    this->conditional->print(os, level+1);
    this->statement->print(os, level+1);
    this->else_statement->print(os, level+1);
}
