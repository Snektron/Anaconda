#ifndef SRC_AST_EXPR_OP_UNARYEXPRESSIONNODE_H_
#define SRC_AST_EXPR_OP_UNARYEXPRESSIONNODE_H_

#include "ast/expr/expressionnode.h"

class UnaryOperatorNode : public ExpressionNode
{
    protected:
        ExpressionNode* op;
        DataTypeBase* type;

        UnaryOperatorNode(ExpressionNode*);
    public:
        virtual ~UnaryOperatorNode();

        virtual void checkTypes(BrainfuckWriter&);
        virtual DataTypeBase* getType();
        virtual void declareLocals(BrainfuckWriter&);
};

#endif
