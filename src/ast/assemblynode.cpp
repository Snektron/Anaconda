#include "ast/node.h"
#include "util/utils.h"

#include <iostream>

AssemblyNode::AssemblyNode(DataTypeBase* datatype, const std::string& assembly, FunctionArguments* arguments):
	datatype(datatype), assembly(assembly), arguments(arguments) {}

AssemblyNode::~AssemblyNode() {}

void AssemblyNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "assembly statement (" << this->assembly << ")" << std::endl;
}

void AssemblyNode::checkTypes(BrainfuckWriter& writer)
{
    this->arguments->checkTypes(writer);
}

DataTypeBase* AssemblyNode::getType()
{
    return this->datatype->copy();
}
