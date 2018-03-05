#ifndef SRC_AST_EXPR_OP_SUBNODE_H_
#define SRC_AST_EXPR_OP_SUBNODE_H_

#include "ast/expr/op/binaryexpressionnode.h"

class SubNode : public BinaryExpressionNode
{
    public:
        SubNode(ExpressionNode*, ExpressionNode*);
        virtual ~SubNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

#endif
