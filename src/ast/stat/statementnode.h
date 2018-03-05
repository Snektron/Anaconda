#ifndef SRC_AST_STAT_STATEMENTNODE_H_
#define SRC_AST_STAT_STATEMENTNODE_H_

#include "ast/node.h"

class StatementNode : public Node
{
    public:
        virtual ~StatementNode() = default;

        virtual void declareLocals(BrainfuckWriter&) = 0;
};

#endif
