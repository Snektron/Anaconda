#include "ast/node.h"

#include <iostream>

ModNode::ModNode(ExpressionNode* lop, ExpressionNode* rop) : lop(lop), rop(rop) {}

ModNode::~ModNode()
{
    delete this->lop;
    delete this->rop;
}

void ModNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "modulo expression" << std::endl;
    this->lop->print(os, level+1);
    this->rop->print(os, level+1);
}
