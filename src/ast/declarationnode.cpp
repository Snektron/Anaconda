#include "ast/node.h"
#include "types/datatype.h"
#include "except/exceptions.h"
#include "common/util.h"
#include "generator/brainfuck.h"

#include <iostream>
#include <memory>
#include <sstream>

DeclarationNode::DeclarationNode(DataTypeBase* type, const std::string& name):
    datatype(type), variable(name) {}

DeclarationNode::~DeclarationNode()
{
    delete this->datatype;
}

void DeclarationNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "declaration statement (" << *this->datatype << " " << this->variable << ")" << std::endl;
}

void DeclarationNode::checkTypes(BrainfuckWriter& writer)
{
    if(writer.getScope() != GLOBAL_SCOPE)
    {
        writer.declareVariable(this->variable, this->datatype);
    }
}

void DeclarationNode::declareGlobals(BrainfuckWriter& writer)
{
    writer.declareVariable(this->variable, this->datatype);
}


void DeclarationNode::generate(BrainfuckWriter& writer)
{
    ///TODO
    writer.unimplemented();
}

DataTypeBase* DeclarationNode::getType()
{
    return this->datatype->copy();
}

void DeclarationNode::declareLocals(BrainfuckWriter& writer)
{
    writer.declareVariable(this->variable, this->datatype);
}
