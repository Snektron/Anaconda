#ifndef SRC_AST_EXPR_OP_BITWISERIGHTSHIFTNODE_H_
#define SRC_AST_EXPR_OP_BITWISERIGHTSHIFTNODE_H_

#include "ast/expr/op/binaryexpressionnode.h"

class BitwiseRightShiftNode : public BinaryExpressionNode
{
    public:
        BitwiseRightShiftNode(ExpressionNode*, ExpressionNode*);
        virtual ~BitwiseRightShiftNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

#endif
