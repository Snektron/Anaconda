#ifndef SRC_AST_STAT_RETURNNODE_H_
#define SRC_AST_STAT_RETURNNODE_H_

#include "ast/stat/statementnode.h"
#include "ast/expr/expressionnode.h"

class ReturnNode : public StatementNode
{
    private:
        ExpressionNode* retval;
    public:
        ReturnNode(ExpressionNode*);
        virtual ~ReturnNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual void declareLocals(BrainfuckWriter&);
};

#endif
