#include "ast/node.h"
#include "generator/brainfuck.h"
#include "except/exceptions.h"

#include <iostream>
#include <sstream>
#include <memory>

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

void AssignmentNode::generate(BrainfuckWriter& writer)
{
    ///TODO
    writer.unimplemented();
}

void AssignmentNode::checkTypes(BrainfuckWriter& writer)
{
    this->lop->checkTypes(writer);
    this->rop->checkTypes(writer);

    std::unique_ptr<DataTypeBase> lop_type(this->lop->getType());
    std::unique_ptr<DataTypeBase> rop_type(this->rop->getType());

    if(!lop_type->equals(*rop_type))
    {
        std::stringstream ss;
        ss << "Type mismatch in assignment: left operand type: " << *lop_type << ", right operand type " << *rop_type << std::endl;
        throw TypeMismatchException(ss.str());
    }
}

DataTypeBase* AssignmentNode::getType()
{
    return this->lop->getType();
}

void AssignmentNode::declareLocals(BrainfuckWriter& writer)
{
    this->lop->declareLocals(writer);
    this->rop->declareLocals(writer);
}
