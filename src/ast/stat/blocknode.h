#ifndef SRC_AST_STAT_BLOCKNODE_H_
#define SRC_AST_STAT_BLOCKNODE_H_

#include "ast/stat/statementnode.h"

class BlockNode : public StatementNode
{
    private:
        StatementNode* content;
    public:
        BlockNode(StatementNode*);
        virtual ~BlockNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual void declareLocals(BrainfuckWriter&);
};

#endif
