#include "ast/expr/op/binaryoperatornode.h"
#include "except/exceptions.h"

#include <sstream>
#include <memory>

BinaryOperatorNode::BinaryOperatorNode(ExpressionNode* lop, ExpressionNode* rop)
    : lop(lop), rop(rop), type(nullptr) {}

BinaryOperatorNode::~BinaryOperatorNode()
{
    delete this->lop;
    delete this->rop;
    delete this->type;
}

void BinaryOperatorNode::checkTypes(BrainfuckWriter& writer)
{
    this->lop->checkTypes(writer);
    this->rop->checkTypes(writer);
    std::unique_ptr<DataTypeBase> lop_type(this->lop->getType());
    std::unique_ptr<DataTypeBase> rop_type(this->rop->getType());

    if(!lop_type->equals(*rop_type))
    {
        std::stringstream ss;
        ss << "Binary operation on different types: ";
        ss << *lop_type << " and " << *rop_type;
        throw TypeMismatchException(ss.str());
    }
    if(!lop_type->supportsArithmetic())
    {
        std::stringstream ss;
        ss << "Binary operation requested on type that does not support arithmetic: ";
        ss << *lop_type;
        throw TypeMismatchException(ss.str());
    }
    this->type = lop_type.release();
}

DataTypeBase* BinaryOperatorNode::getType()
{
    return this->type->copy();
}

void BinaryOperatorNode::declareLocals(BrainfuckWriter& writer)
{
    this->lop->declareLocals(writer);
    this->rop->declareLocals(writer);
}
