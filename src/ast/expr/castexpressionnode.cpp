#include "ast/expr/castexpressionnode.h"
#include "types/datatype.h"
#include "generator/brainfuck.h"
#include "except/exceptions.h"
#include "common/util.h"

#include <iostream>
#include <memory>
#include <sstream>

CastExpressionNode::CastExpressionNode(ExpressionNode* expression, DataTypeBase* desired_type):
    expression(expression), desired_type(desired_type) {}

CastExpressionNode::~CastExpressionNode()
{
    delete this->expression;
    delete this->desired_type;
}

void CastExpressionNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "cast expression (" << *this->desired_type << ")" << std::endl;
    this->expression->print(os, level+1);
}

void CastExpressionNode::generate(BrainfuckWriter& writer)
{
    ///TODO
    writer.unimplemented();
}

void CastExpressionNode::checkTypes(BrainfuckWriter& writer)
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

DataTypeBase* CastExpressionNode::getType()
{
    return this->desired_type->copy();
}

void CastExpressionNode::declareLocals(BrainfuckWriter& writer)
{
    UNUSED(writer);
}
