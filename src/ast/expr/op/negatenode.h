#ifndef SRC_AST_EXPR_OP_NEGATENODE_H_
#define SRC_AST_EXPR_OP_NEGATENODE_H_

#include "ast/expr/op/unaryexpressionnode.h"

class NegateNode : public UnaryExpressionNode
{
    public:
        NegateNode(ExpressionNode*);
        virtual ~NegateNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

#endif
