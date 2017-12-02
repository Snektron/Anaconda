#include "ast/node.h"

#include <iostream>

FunctionCallNode::FunctionCallNode(const std::string& name, FunctionArguments* arguments) : function_name(name), arguments(arguments) {}

FunctionCallNode::~FunctionCallNode()
{
    delete this->arguments;
}

void FunctionCallNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "function call (" << this->function_name << ")" << std::endl;
    this->arguments->print(os, level+1);
}
