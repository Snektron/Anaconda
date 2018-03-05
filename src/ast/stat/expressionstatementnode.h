#ifndef SRC_AST_STAT_EXPRESSIONSTATEMENTNODE_H_
#define SRC_AST_STAT_EXPRESSIONSTATEMENTNODE_H_

#include "ast/stat/statementnode.h"
#include "ast/expr/expressionnode.h"

class ExpressionStatementNode : public StatementNode
{
    private:
        ExpressionNode* content;
    public:
        ExpressionStatementNode(ExpressionNode*);
        virtual ~ExpressionStatementNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual void declareLocals(BrainfuckWriter&);
};

#endif
