#include "ast/node.h"
#include "types/datatype.h"
#include "generator/brainfuck.h"
#include "except/exceptions.h"

#include <iostream>
#include <memory>
#include <sstream>

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

void GlobalDeclarationNode::declareGlobals(BrainfuckWriter& writer) const
{
    writer.declareVariable(this->variable_name, this->variable_type);
}

void GlobalDeclarationNode::checkTypes(BrainfuckWriter& writer)
{
    if(this->initializer != nullptr)
    {
        this->initializer->checkTypes(writer);

        std::unique_ptr<DataTypeBase> sub_type(initializer->getType());   
        if(!variable_type->equals(*sub_type))
        {
            std::stringstream ss;
            ss << "Type mismatch for global declaration for variable " << this->variable_name << ", failed to convert from " << *sub_type << " to " << *this->variable_type;
            throw TypeCheckException(ss.str());
        }
    }
}
