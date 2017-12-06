#include "ast/node.h"
#include "generator/brainfuck.h"

#include <iostream>

GlobalNode::GlobalNode(const std::vector<GlobalElementNode*>& elements):
    elements(elements) {}

GlobalNode::~GlobalNode()
{
    for(auto& it : this->elements)
        delete it;
}

void GlobalNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "global namespace" << std::endl;
    
    for(auto& it : this->elements)
        it->print(os, level+1);
}

void GlobalNode::declareGlobals(BrainfuckWriter& writer)
{
    size_t old_scope = writer.getScope();
    writer.switchScope(GLOBAL_SCOPE);

    for(auto& it : this->elements)
        it->declareGlobals(writer);

    writer.switchScope(old_scope);
}

void GlobalNode::checkTypes(BrainfuckWriter& writer)
{
    size_t old_scope = writer.getScope();
    writer.switchScope(GLOBAL_SCOPE);
    
    for(auto& it : this->elements)
        it->declareGlobals(writer);

    writer.switchScope(old_scope);
}
