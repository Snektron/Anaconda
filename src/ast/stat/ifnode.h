#ifndef SRC_AST_STAT_IFNODE_H_
#define SRC_AST_STAT_IFNODE_H_

#include "ast/stat/statementnode.h"
#include "ast/expr/expressionnode.h"

class IfNode : public StatementNode
{
    private:
        ExpressionNode* conditional;
        StatementNode* statement;
    public:
        IfNode(ExpressionNode*, StatementNode*);
        virtual ~IfNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual void declareLocals(BrainfuckWriter&);
};

#endif
