#include "ast/node.h"
#include "types/datatype.h"
#include "except/exceptions.h"

#include <iostream>
#include <memory>
#include <sstream>

DeclarationNode::DeclarationNode(DataTypeBase* type, VariableNode* name):
	datatype(type), variable(name) {}

DeclarationNode::~DeclarationNode()
{
    delete this->datatype;
    delete this->variable;
}

void DeclarationNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "declaration statement (" << *this->variable_type << ")" << std::endl;
    this->variable->print(os, level + 1);
}

void DeclarationNode::checkTypes(BrainfuckWriter& writer)
{
    if(this->initializer != nullptr)
    {
        this->initializer->checkTypes(writer);
        std::unique_ptr<DataTypeBase> init_datatype(this->initializer->getType());
        if(!init_datatype->equals(*variable_type))
        {
            std::stringstream ss;
            ss << "Type mismatch in declaration node initializer: trying to assign expression of type"
                << *init_datatype << " to " << *this->variable_type;
            throw TypeMismatchException(ss.str());
        }
    }
}
