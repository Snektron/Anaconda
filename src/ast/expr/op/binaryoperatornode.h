#ifndef SRC_AST_EXPR_OP_BINARYEXPRESSIONNODE_H_
#define SRC_AST_EXPR_OP_BINARYEXPRESSIONNODE_H_

#include "ast/expr/expressionnode.h"

class BinaryOperatorNode : public ExpressionNode
{
    protected:
        ExpressionNode* lop;
        ExpressionNode* rop;
        DataTypeBase* type;

        BinaryOperatorNode(ExpressionNode*, ExpressionNode*);
    public:
        virtual ~BinaryOperatorNode();

        virtual void checkTypes(BrainfuckWriter&);
        virtual DataTypeBase* getType();
        virtual void declareLocals(BrainfuckWriter&);
};

#endif
