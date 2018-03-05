#ifndef SRC_AST_EXPR_OP_UNARYEXPRESSIONNODE_H_
#define SRC_AST_EXPR_OP_UNARYEXPRESSIONNODE_H_

#include "ast/expr/expressionnode.h"

class UnaryExpressionNode : public ExpressionNode
{
    protected:
        ExpressionNode* op;
        DataTypeBase* type;

        UnaryExpressionNode(ExpressionNode*);
    public:
        virtual ~UnaryExpressionNode();

        virtual void checkTypes(BrainfuckWriter&);
        virtual DataTypeBase* getType();
        virtual void declareLocals(BrainfuckWriter&);
};

#endif
