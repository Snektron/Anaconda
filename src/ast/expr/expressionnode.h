#ifndef SRC_AST_EXPR_EXPRESSIONNODE_H_
#define SRC_AST_EXPR_EXPRESSIONNODE_H_

#include "ast/node.h"
#include "types/datatype.h"

class ExpressionNode : public Node
{
    public:
        virtual ~ExpressionNode() = default;

        virtual DataTypeBase* getType() = 0;
        virtual void declareLocals(BrainfuckWriter&) = 0;
};

#endif
