#include "ast/node.h"
#include "generator/brainfuck.h"

#include <iostream>

ArgumentListNode::ArgumentListNode(const std::vector<ExpressionNode*>& arguments):
    arguments(arguments) {}

ArgumentListNode::~ArgumentListNode()
{
    for(auto& it : this->arguments)
        delete it;
}

void ArgumentListNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "function arguments" << std::endl;
    for(auto& it : this->arguments)
        it->print(os, level+1);
}

void ArgumentListNode::generate(BrainfuckWriter& writer)
{
    ///TODO
    writer.unimplemented();
}

void ArgumentListNode::declareGlobals(BrainfuckWriter& writer)
{
    for(ExpressionNode* expression : this->arguments)
        expression->declareGlobals(writer);
}

void ArgumentListNode::checkTypes(BrainfuckWriter& writer)
{
    for(ExpressionNode* expression : this->arguments)
        expression->checkTypes(writer);
}

std::vector<DataTypeBase*> ArgumentListNode::getArgumentTypes()
{
    std::vector<DataTypeBase*> result;
    for(ExpressionNode* expr : this->arguments)
        result.push_back(expr->getType());
    return result;
}
