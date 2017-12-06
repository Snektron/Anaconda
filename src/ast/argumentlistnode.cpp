#include "ast/node.h"

#include <iostream>

ArgumentListNode::ArgumentListNode(const std::vector<ExpressionNode*>& arguments):
	arguments(arguments) {}

ArgumentListNode::~ArgumentListNode()
{
    for(auto& it : this->arguments)
        delete it;
}

void ArgumentListNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "function arguments" << std::endl;
    for(auto& it : this->arguments)
        it->print(os, level+1);
}
