#include "ast/node.h"
#include "except/exceptions.h"

#include <sstream>
#include <memory>

BinaryExpressionNode::BinaryExpressionNode(ExpressionNode* lop, ExpressionNode* rop)
    : lop(lop), rop(rop), type(nullptr) {}

BinaryExpressionNode::~BinaryExpressionNode()
{
    delete this->lop;
    delete this->rop;
    delete this->type;
}

void BinaryExpressionNode::checkTypes(BrainfuckWriter& writer)
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

DataTypeBase* BinaryExpressionNode::getType()
{
    return this->type->copy();
}
