#ifndef SRC_AST_EXPR_OP_BITWISEORNODE_H_
#define SRC_AST_EXPR_OP_BITWISEORNODE_H_

#include "ast/expr/op/binaryoperatornode.h"

class BitwiseOrNode : public BinaryOperatorNode
{
    public:
        BitwiseOrNode(ExpressionNode*, ExpressionNode*);
        virtual ~BitwiseOrNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

#endif
