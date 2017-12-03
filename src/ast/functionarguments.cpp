#include "ast/node.h"

#include <iostream>

FunctionArguments::FunctionArguments(const std::vector<ExpressionNode*>& arguments):
	arguments(arguments) {}

FunctionArguments::~FunctionArguments()
{
    for(auto& it : this->arguments)
        delete it;
}

void FunctionArguments::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "function arguments" << std::endl;
    for(auto& it : this->arguments)
        it->print(os, level+1);
}
