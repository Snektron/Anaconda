#include "ast/node.h"
#include "types/datatype.h"
#include "generator/brainfuck.h"
#include "except/exceptions.h"

#include <iostream>

StructureDefinitionNode::StructureDefinitionNode(const std::string& name, const std::map<std::string, DataTypeBase*>& members) : name(name), members(members), type(new DataType<DataTypeClass::STRUCT_FORWARD>(name)) {}

StructureDefinitionNode::~StructureDefinitionNode()
{
    for(auto& it : this->members)
        delete it.second;
}

void StructureDefinitionNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "structure definition (" << this->name << ")" << std::endl;
    for(auto& it : this->members)
    {
        this->printIndent(os, level+1);
        os << it.first << "->" << *it.second << std::endl;
    }
}

void StructureDefinitionNode::declareGlobals(BrainfuckWriter& writer) const
{
    writer.declareStructure(this->name, this->members);
}

void StructureDefinitionNode::checkTypes(BrainfuckWriter& writer)
{
    for(auto& it : this->members)
    {
        if(it.second->equals(*this->type))
            throw RecursiveTypeException("Structure " + this->name + " contains itself in member " + it.first);
    }
}
