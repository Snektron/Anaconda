#include "ast/fieldlistnode.h"
#include "types/datatype.h"
#include "common/util.h"
#include "common/format.h"
#include <iostream>

FieldListNode::FieldListNode(const std::vector<Field>& arguments):
    arguments(arguments) {}

FieldListNode::~FieldListNode()
{}

void FieldListNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    fmt::fprintf(os, "function parameters (", this->arguments, ")");
}

std::vector<Field>& FieldListNode::getParameters()
{
    return this->arguments;
}

void FieldListNode::checkTypes(BrainfuckWriter& writer)
{
    UNUSED(writer);
}

void FieldListNode::generate(BrainfuckWriter& writer)
{
    UNUSED(writer);
}
