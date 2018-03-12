#ifndef SRC_AST_EXPR_OP_MULNODE_H_
#define SRC_AST_EXPR_OP_MULNODE_H_

#include "ast/expr/op/binaryoperatornode.h"

class MulNode : public BinaryOperatorNode
{
    public:
        MulNode(ExpressionNode*, ExpressionNode*);
        virtual ~MulNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

#endif
