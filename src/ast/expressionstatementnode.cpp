#include "ast/node.h"
#include "generator/brainfuck.h"

#include <iostream>
#include <memory>

ExpressionStatementNode::ExpressionStatementNode(ExpressionNode* node):
    content(node) {}

ExpressionStatementNode::~ExpressionStatementNode()
{
    delete this->content;
}

void ExpressionStatementNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "expression statement" << std::endl;
    this->content->print(os, level+1);
}

void ExpressionStatementNode::checkTypes(BrainfuckWriter& writer)
{
    this->content->checkTypes(writer);
}

void ExpressionStatementNode::generate(BrainfuckWriter& writer)
{
    this->content->generate(writer);
    std::unique_ptr<DataTypeBase> datatype(this->content->getType());
    writer.pop(datatype.get());
}

void ExpressionStatementNode::declareLocals(BrainfuckWriter& writer)
{
    this->content->declareLocals(writer);
}
