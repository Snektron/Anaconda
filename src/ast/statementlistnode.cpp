#include "ast/node.h"

StatementListNode::StatementListNode(StatementNode* first, StatementNode* second):
	first(first), second(second) {}

StatementListNode::~StatementListNode()
{
    delete this->first;
    delete this->second;
}

void StatementListNode::print(std::ostream& os, size_t level) const
{
    this->first->print(os, level);
    this->second->print(os, level);
}

void StatementListNode::checkTypes(BrainfuckWriter& writer)
{
    this->first->checkTypes(writer);
    this->second->checkTypes(writer);
}
