#include "ast/node.h"
#include "types/datatype.h"

#include <iostream>

GlobalDeclarationNode::GlobalDeclarationNode(DataTypeBase* type, const std::string& name) : variable_type(type), variable_name(name), initializer(nullptr) {}
GlobalDeclarationNode::GlobalDeclarationNode(DataTypeBase* type, const std::string& name, ExpressionNode* initializer) : variable_type(type), variable_name(name), initializer(initializer) {}

GlobalDeclarationNode::~GlobalDeclarationNode()
{
    delete this->variable_type;
    delete this->initializer;
}

void GlobalDeclarationNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "global declaration (" << this->variable_name << " -> " << *this->variable_type << ")" << std::endl;
    if(this->initializer)
        this->initializer->print(os, level+1);
}
