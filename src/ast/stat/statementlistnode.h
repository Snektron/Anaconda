#ifndef SRC_AST_STAT_STATEMENTLISTNODE_H_
#define SRC_AST_STAT_STATEMENTLISTNODE_H_

#include "ast/stat/statementnode.h"

class StatementListNode : public StatementNode
{
    private:
        StatementNode* first;
        StatementNode* second;
    public:
        StatementListNode(StatementNode*, StatementNode*);
        virtual ~StatementListNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual void declareLocals(BrainfuckWriter&);
};

#endif
