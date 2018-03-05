#include "ast/expr/assemblynode.h"
#include "common/util.h"
#include "generator/brainfuck.h"

#include <iostream>

AssemblyNode::AssemblyNode(DataTypeBase* datatype, const std::string& assembly, ArgumentListNode* arguments):
    datatype(datatype), assembly(assembly), arguments(arguments) {}

AssemblyNode::~AssemblyNode()
{
    delete this->datatype;
}

void AssemblyNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "assembly statement -> " << *this->datatype << " (" << this->assembly << ")" << std::endl;
    this->arguments->print(os, level+1);
}

void AssemblyNode::checkTypes(BrainfuckWriter& writer)
{
    this->arguments->checkTypes(writer);
}

DataTypeBase* AssemblyNode::getType()
{
    return this->datatype->copy();
}

void AssemblyNode::generate(BrainfuckWriter& writer)
{
    //Zero initialized return value
    size_t stack_location = writer.getStackLocation();
    writer.push(this->datatype);
    size_t new_stack_location = writer.getStackLocation();
    for(size_t i = 0; i != new_stack_location; ++i)
    {
        writer.moveStackPointerTo(stack_location + i);
        writer.clearByte();
    }
    writer.moveStackPointerTo(new_stack_location);
    //Arguments
    this->arguments->generate(writer);
    //Assembly code
    writer.copyAssembly(this->assembly);
    //Argument cleanup
    writer.moveStackPointerTo(new_stack_location);
}

void AssemblyNode::declareLocals(BrainfuckWriter& writer)
{
    UNUSED(writer);
}
