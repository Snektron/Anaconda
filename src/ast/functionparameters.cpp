#include "ast/node.h"
#include "types/datatype.h"
#include "util/utils.h"

#include <iostream>

FieldListNode::FieldListNode(const std::vector<Field>& arguments):
	arguments(arguments) {}

FieldListNode::~FieldListNode()
{

}

void FieldListNode::print(std::ostream& os, size_t level) const
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

std::vector<Field>& FieldListNode::getParameters()
{
    return this->arguments;
}

void FieldListNode::checkTypes(BrainfuckWriter& writer)
{
    UNUSED(writer);
}
