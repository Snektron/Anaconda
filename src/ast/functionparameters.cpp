#include "ast/node.h"
#include "types/datatype.h"

#include <iostream>

FunctionParameters::FunctionParameters(const std::vector<std::pair<std::string, DataTypeBase*>>& arguments) : arguments(arguments) {}

FunctionParameters::~FunctionParameters()
{
    for(auto& it : this->arguments)
    {
        delete it.second;
    }
}

void FunctionParameters::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "function parameters (";
    bool first = true;
    for(auto& it : this->arguments)
    {
        if(!first)
            os << ", ";
        else
            first = false;
        os << it.first << ": " << *it.second;
    }
    os << ")" << std::endl;
}

std::vector<std::pair<std::string, DataTypeBase*>>& FunctionParameters::getParameters()
{
    return this->arguments;
}

void FunctionParameters::checkTypes(BrainfuckWriter& writer)
{
    
}
