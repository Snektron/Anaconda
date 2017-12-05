#include "ast/node.h"

#include <iostream>

AssignmentNode::AssignmentNode(ExpressionNode* name, ExpressionNode* expr):
    lop(name), rop(expr) {}

AssignmentNode::~AssignmentNode()
{
    delete this->lop;
    delete this->rop;
}

void AssignmentNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "assignment expression" << std::endl;
    this->lop->print(os, level+1);
    this->rop->print(os, level+1);
}
