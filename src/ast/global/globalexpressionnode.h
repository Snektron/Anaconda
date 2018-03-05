#ifndef SRC_AST_GLOBAL_GLOBALEXPRESSIONNODE_H_
#define SRC_AST_GLOBAL_GLOBALEXPRESSIONNODE_H_

#include "ast/global/globalelementnode.h"
#include "ast/expr/expressionnode.h"

class GlobalExpressionNode : public GlobalElementNode
{
    private:
        ExpressionNode* expression;
    public:
        GlobalExpressionNode(ExpressionNode*);
        virtual ~GlobalExpressionNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareGlobals(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
};

#endif
