#ifndef SRC_AST_EXPR_CASTEXPRESSION_H_
#define SRC_AST_EXPR_CASTEXPRESSION_H_

#include "ast/expr/expressionnode.h"

class CastExpressionNode : public ExpressionNode
{
    private:
        ExpressionNode* expression;
        DataTypeBase* desired_type;
    public:
        CastExpressionNode(ExpressionNode*, DataTypeBase*);
        virtual ~CastExpressionNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual DataTypeBase* getType();
        virtual void declareLocals(BrainfuckWriter&);
};

#endif
