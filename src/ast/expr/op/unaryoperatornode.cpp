#include "ast/expr/op/unaryoperatornode.h"
#include "except/exceptions.h"

#include <sstream>
#include <memory>

UnaryOperatorNode::UnaryOperatorNode(ExpressionNode* op)
    : op(op) {}

UnaryOperatorNode::~UnaryOperatorNode()
{
    delete this->op;
    delete this->type;
}

void UnaryOperatorNode::checkTypes(BrainfuckWriter& writer)
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

DataTypeBase* UnaryOperatorNode::getType()
{
    return this->type->copy();
}

void UnaryOperatorNode::declareLocals(BrainfuckWriter& writer)
{
    this->op->declareLocals(writer);
}
