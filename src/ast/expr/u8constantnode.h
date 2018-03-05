#ifndef SRC_AST_EXPR_U8CONSTANTNODE_H_
#define SRC_AST_EXPR_U8CONSTANTNODE_H_

#include <cstdint>
#include "ast/expr/expressionnode.h"

class U8ConstantNode : public ExpressionNode
{
    private:
        uint8_t value;
    public:
        U8ConstantNode(uint8_t);
        virtual ~U8ConstantNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual DataTypeBase* getType();
        virtual void declareLocals(BrainfuckWriter&);
};

#endif
