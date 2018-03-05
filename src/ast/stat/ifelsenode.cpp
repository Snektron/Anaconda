#include "ast/stat/ifelsenode.h"
#include "except/exceptions.h"
#include "generator/brainfuck.h"

#include <iostream>
#include <memory>

IfElseNode::IfElseNode(ExpressionNode* conditional, StatementNode* statement, StatementNode* else_statement):
    conditional(conditional), statement(statement), else_statement(else_statement) {}

IfElseNode::~IfElseNode()
{
    delete this->conditional;
    delete this->statement;
    delete this->else_statement;
}

void IfElseNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "if-else statement" << std::endl;
    this->conditional->print(os, level+1);
    this->statement->print(os, level+1);
    this->else_statement->print(os, level+1);
}

void IfElseNode::checkTypes(BrainfuckWriter& writer)
{
    this->conditional->checkTypes(writer);
    this->statement->checkTypes(writer);
    this->else_statement->checkTypes(writer);

    std::unique_ptr<DataTypeBase> cond_type(this->conditional->getType());
    if(!cond_type->isBoolean())
        throw TypeMismatchException("Conditional for if-else statement was not convertable to boolean");
}

void IfElseNode::generate(BrainfuckWriter& writer)
{
    ///TODO
    writer.unimplemented();
}

void IfElseNode::declareLocals(BrainfuckWriter& writer)
{
    this->conditional->declareLocals(writer);
    this->statement->declareLocals(writer);
    this->else_statement->declareLocals(writer);
}
