#include "ast/stat/ifnode.h"
#include "except/exceptions.h"
#include "generator/brainfuck.h"

#include <iostream>
#include <memory>

IfNode::IfNode(ExpressionNode* conditional, StatementNode* statement):
    conditional(conditional), statement(statement) {}

IfNode::~IfNode()
{
    delete this->conditional;
    delete this->statement;
}

void IfNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "if statement" << std::endl;
    this->conditional->print(os, level+1);
    this->statement->print(os, level+1);
}

void IfNode::checkTypes(BrainfuckWriter& writer)
{
    this->conditional->checkTypes(writer);
    this->statement->checkTypes(writer);

    std::unique_ptr<DataTypeBase> cond_type(this->conditional->getType());
    if(!cond_type->isBoolean())
        throw TypeMismatchException("Conditional for if statement was not convertable to boolean");
}

void IfNode::generate(BrainfuckWriter& writer)
{
    ///TODO
    writer.unimplemented();
}

void IfNode::declareLocals(BrainfuckWriter& writer)
{
    this->conditional->declareLocals(writer);
    this->statement->declareLocals(writer);
}
