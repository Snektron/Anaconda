#include "ast/node.h"

#include <iostream>

VariableNode::VariableNode(const std::string& variable):
	variable(variable) {}

VariableNode::~VariableNode() {}

void VariableNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "variable (" << this->variable << ")" << std::endl;
}
