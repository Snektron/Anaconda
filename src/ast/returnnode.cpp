#include "ast/node.h"

#include <iostream>

ReturnNode::ReturnNode(ExpressionNode* retval):
	retval(retval) {}

ReturnNode::~ReturnNode()
{
    delete this->retval;
}

void ReturnNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "return statement" << std::endl;
    this->retval->print(os, level+1);
}
