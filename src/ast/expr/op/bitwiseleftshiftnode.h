#ifndef SRC_AST_EXPR_OP_BITWISELEFTSHIFTNODE_H_
#define SRC_AST_EXPR_OP_BITWISELEFTSHIFTNODE_H_

#include "ast/expr/op/binaryexpressionnode.h"

class BitwiseLeftShiftNode : public BinaryExpressionNode
{
    public:
        BitwiseLeftShiftNode(ExpressionNode*, ExpressionNode*);
        virtual ~BitwiseLeftShiftNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

#endif
