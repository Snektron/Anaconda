#include "ast/node.h"

#include <iostream>

AssemblyNode::AssemblyNode(const std::string& assembly):
	assembly(assembly) {}

AssemblyNode::~AssemblyNode() {}

void AssemblyNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "assembly statement (" << this->assembly << ")" << std::endl;
}
