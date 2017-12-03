#include "ast/node.h"

#include <iostream>

AssignmentNode::AssignmentNode(const std::string& name, ExpressionNode* expr):
	variable_name(name), expression(expr) {}

AssignmentNode::~AssignmentNode()
{
    delete this->expression;
}

void AssignmentNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "assignment statement (" << this->variable_name << ")" << std::endl;
    this->expression->print(os, level+1);
}
