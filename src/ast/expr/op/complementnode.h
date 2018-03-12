#ifndef SRC_AST_EXPR_OP_COMPLEMENTNODE_H_
#define SRC_AST_EXPR_OP_COMPLEMENTNODE_H_

#include "ast/expr/op/unaryoperatornode.h"

class ComplementNode : public UnaryOperatorNode
{
    public:
        ComplementNode(ExpressionNode*);
        virtual ~ComplementNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

#endif
