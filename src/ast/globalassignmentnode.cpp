#include "ast/node.h"
#include "ast/datatype.h"

#include <iostream>

GlobalAssignmentNode::GlobalAssignmentNode(DataTypeBase* type, const std::string& name) : variable_type(type), variable_name(name), initializer(nullptr) {}
GlobalAssignmentNode::GlobalAssignmentNode(DataTypeBase* type, const std::string& name, ExpressionNode* initializer) : variable_type(type), variable_name(name), initializer(initializer) {}

GlobalAssignmentNode::~GlobalAssignmentNode()
{
    delete this->variable_type;
    delete this->initializer;
}

void GlobalAssignmentNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "global assignment (" << this->variable_name << " -> " << *this->variable_type << ")" << std::endl;
    if(this->initializer)
        this->initializer->print(os, level+1);
}
