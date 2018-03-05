#ifndef SRC_AST_EXPR_OP_BITWISEXORNODE_H_
#define SRC_AST_EXPR_OP_BITWISEXORNODE_H_

#include "ast/expr/op/binaryexpressionnode.h"

class BitwiseXorNode : public BinaryExpressionNode
{
    public:
        BitwiseXorNode(ExpressionNode*, ExpressionNode*);
        virtual ~BitwiseXorNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

#endif
