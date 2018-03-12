#ifndef SRC_AST_EXPR_OP_BITWISEANDNODE_H_
#define SRC_AST_EXPR_OP_BITWISEANDNODE_H_

#include "ast/expr/op/binaryoperatornode.h"

class BitwiseAndNode : public BinaryOperatorNode
{
    public:
        BitwiseAndNode(ExpressionNode*, ExpressionNode*);
        virtual ~BitwiseAndNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

#endif
