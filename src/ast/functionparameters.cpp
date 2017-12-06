#include "ast/node.h"
#include "types/datatype.h"
#include "util/utils.h"

#include <iostream>

FunctionParameters::FunctionParameters(const std::vector<Field>& arguments):
	arguments(arguments) {}

FunctionParameters::~FunctionParameters()
{

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
        os << it.getName() << ": " << *it.getType();
    }
    os << ")" << std::endl;
}

std::vector<Field>& FunctionParameters::getParameters()
{
    return this->arguments;
}

void FunctionParameters::checkTypes(BrainfuckWriter& writer)
{
    UNUSED(writer);
}
