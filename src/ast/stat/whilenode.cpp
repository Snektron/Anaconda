#include "ast/stat/whilenode.h"
#include "except/exceptions.h"
#include "generator/brainfuck.h"

#include <iostream>
#include <memory>

WhileNode::WhileNode(ExpressionNode* conditional, StatementNode* statement):
    conditional(conditional), statement(statement) {}

WhileNode::~WhileNode()
{
    delete this->conditional;
    delete this->statement;
}

void WhileNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "while statement" << std::endl;
    this->conditional->print(os, level+1);
    this->statement->print(os, level+1);
}

void WhileNode::checkTypes(BrainfuckWriter& writer)
{
    this->conditional->checkTypes(writer);
    this->statement->checkTypes(writer);

    std::unique_ptr<DataTypeBase> cond_type(this->conditional->getType());
    if(!cond_type->isBoolean())
        throw TypeMismatchException("Cannot convert conditional in while-loop to a boolean");
}

void WhileNode::generate(BrainfuckWriter& writer)
{
    ///TODO
    writer.unimplemented();
}

void WhileNode::declareLocals(BrainfuckWriter& writer)
{
    this->conditional->declareLocals(writer);
    this->statement->declareLocals(writer);
}
