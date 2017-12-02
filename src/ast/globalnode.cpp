#include "ast/node.h"

#include <iostream>

GlobalNode::GlobalNode(const std::vector<GlobalElementNode*>& elements) : elements(elements) {}

GlobalNode::~GlobalNode()
{
    for(auto& it : this->elements)
        delete it;
}

void GlobalNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "global namespace";
    
    for(auto& it : this->elements)
        it->print(os, level+1);
}
