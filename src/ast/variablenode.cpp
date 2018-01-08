#include "ast/node.h"
#include "generator/brainfuck.h"
#include "except/exceptions.h"
#include "util/utils.h"

#include <iostream>
#include <memory>
#include <sstream>

VariableNode::VariableNode(const std::string& variable):
    variable(variable), datatype(nullptr) {}

VariableNode::~VariableNode()
{
    delete this->datatype;
}

void VariableNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "variable (" << this->variable << ")" << std::endl;
}

void VariableNode::generate(BrainfuckWriter& writer)
{
    std::unique_ptr<VariableDefinition> variable(writer.getDeclaredVariable(this->variable));
    std::unique_ptr<DataTypeBase> datatype(variable->dataType());
    writer.loadValue(variable->location(), datatype->size(writer));
}

void VariableNode::checkTypes(BrainfuckWriter& writer)
{
    std::unique_ptr<VariableDefinition> variable(writer.getDeclaredVariable(this->variable));
    if(variable == nullptr)
    {
        std::stringstream ss;
        ss << "Use of undeclared variable " << this->variable;
        throw TypeCheckException(ss.str());
    }

    this->datatype = variable->dataType();
}

DataTypeBase* VariableNode::getType()
{
    return this->datatype;
}

void VariableNode::declareLocals(BrainfuckWriter& writer)
{
    UNUSED(writer);
}
