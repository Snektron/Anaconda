#ifndef SRC_AST_EXPR_OP_ADDNODE_H_
#define SRC_AST_EXPR_OP_ADDNODE_H_

#include "ast/expr/op/binaryexpressionnode.h"

class AddNode : public BinaryExpressionNode
{
    public:
        AddNode(ExpressionNode*, ExpressionNode*);
        virtual ~AddNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

#endif
