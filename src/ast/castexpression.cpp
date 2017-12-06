#include "ast/node.h"
#include "types/datatype.h"

#include <iostream>

CastExpression::CastExpression(ExpressionNode* expression, DataTypeBase* desired_type):
    expression(expression), desired_type(desired_type) {}

CastExpression::~CastExpression()
{
    delete this->expression;    
    delete this->desired_type;
}

void CastExpression::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "cast expression (" << *this->desired_type << ")" << std::endl;
    this->expression->print(os, level+1);
}
