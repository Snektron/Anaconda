#include "ast/stat/blocknode.h"
#include "generator/brainfuck.h"
#include "common/util.h"

#include <iostream>

BlockNode::BlockNode(StatementNode* content):
    content(content) {}

BlockNode::~BlockNode()
{
    delete this->content;
}

void BlockNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "block node" << std::endl;
    this->content->print(os, level+1);
}

void BlockNode::checkTypes(BrainfuckWriter& writer)
{
    writer.enterFrame();
    this->content->checkTypes(writer);
    writer.exitFrame();
}

void BlockNode::generate(BrainfuckWriter& writer)
{
    writer.enterFrame();
    this->content->declareLocals(writer);

    writer.makeStackFrame();
    this->content->generate(writer);
    writer.destroyStackFrame();

    writer.exitFrame();
}

void BlockNode::declareLocals(BrainfuckWriter& writer)
{
    UNUSED(writer);
}
