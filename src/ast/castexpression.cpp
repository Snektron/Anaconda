#include "ast/node.h"
#include "types/datatype.h"
#include "generator/brainfuck.h"
#include "except/exceptions.h"
#include "common/util.h"

#include <iostream>
#include <memory>
#include <sstream>

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

void CastExpression::generate(BrainfuckWriter& writer)
{
    ///TODO
    writer.unimplemented();
}

void CastExpression::checkTypes(BrainfuckWriter& writer)
{
    this->expression->checkTypes(writer);

    std::unique_ptr<DataTypeBase> expression_type(this->expression->getType());

    if(!this->desired_type->canCastFrom(*expression_type))
    {
        std::stringstream ss;
        ss << "Unable to cast from type " << *expression_type << " to " << *this->desired_type << std::endl;
        throw TypeMismatchException(ss.str());
    }
}

DataTypeBase* CastExpression::getType()
{
    return this->desired_type->copy();
}

void CastExpression::declareLocals(BrainfuckWriter& writer)
{
    UNUSED(writer);
}
