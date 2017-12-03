#include "ast/node.h"
#include "types/datatype.h"

#include <iostream>

DeclarationNode::DeclarationNode(DataTypeBase* type, const std::string& name):
	variable_type(type), variable_name(name), initializer(nullptr) {}

DeclarationNode::DeclarationNode(DataTypeBase* type, const std::string& name, ExpressionNode* initializer):
	variable_type(type), variable_name(name), initializer(initializer) {}

DeclarationNode::~DeclarationNode()
{
    delete this->variable_type;
    delete this->initializer;
}

void DeclarationNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "declaration statement (" << this->variable_name << " -> " << *this->variable_type << ")" << std::endl;
    if(this->initializer)
        this->initializer->print(os, level+1);
}
