#ifndef SRC_AST_EXPR_OP_MODNODE_H_
#define SRC_AST_EXPR_OP_MODNODE_H_

#include "ast/expr/op/binaryexpressionnode.h"

class ModNode : public BinaryExpressionNode
{
    public:
        ModNode(ExpressionNode*, ExpressionNode*);
        virtual ~ModNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

#endif
