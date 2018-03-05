#include "ast/expr/op/unaryexpressionnode.h"
#include "except/exceptions.h"

#include <sstream>
#include <memory>

UnaryExpressionNode::UnaryExpressionNode(ExpressionNode* op)
    : op(op) {}

UnaryExpressionNode::~UnaryExpressionNode()
{
    delete this->op;
    delete this->type;
}

void UnaryExpressionNode::checkTypes(BrainfuckWriter& writer)
{
    this->op->checkTypes(writer);

    std::unique_ptr<DataTypeBase> op_type(this->op->getType());
    if(!op_type->supportsArithmetic())
    {
        std::stringstream ss;
        ss << "Unary operation requested on type that does not support arithmetic: ";
        ss << *op_type;
        throw TypeMismatchException(ss.str());
    }
    this->type = op_type.release();
}

DataTypeBase* UnaryExpressionNode::getType()
{
    return this->type->copy();
}

void UnaryExpressionNode::declareLocals(BrainfuckWriter& writer)
{
    this->op->declareLocals(writer);
}
