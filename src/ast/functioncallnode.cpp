#include "ast/node.h"

#include <iostream>

FunctionCallNode::FunctionCallNode(const std::string& function_var, ArgumentListNode* arguments):
    function_var(function_var), arguments(arguments) {}

FunctionCallNode::~FunctionCallNode()
{
    delete this->arguments;
}

void FunctionCallNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "function call (" << this->function_var << ")" << std::endl;
    this->arguments->print(os, level+1);
}
