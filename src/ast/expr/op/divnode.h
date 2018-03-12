#ifndef SRC_AST_EXPR_OP_DIVNODE_H_
#define SRC_AST_EXPR_OP_DIVNODE_H_

#include "ast/expr/op/binaryoperatornode.h"

class DivNode : public BinaryOperatorNode
{
    public:
        DivNode(ExpressionNode*, ExpressionNode*);
        virtual ~DivNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

#endif
