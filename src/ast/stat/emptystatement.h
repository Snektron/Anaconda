#ifndef SRC_AST_STAT_EMPTYSTATEMENTNODE_H_
#define SRC_AST_STAT_EMPTYSTATEMENTNODE_H_

#include "ast/stat/statementnode.h"

class EmptyStatementNode : public StatementNode
{
    public:
        EmptyStatementNode();
        virtual ~EmptyStatementNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual void declareLocals(BrainfuckWriter&);
};

#endif
