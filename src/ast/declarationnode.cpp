#include "ast/node.h"
#include "types/datatype.h"
#include "except/exceptions.h"
#include "util/utils.h"

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
    UNUSED(writer);
}
