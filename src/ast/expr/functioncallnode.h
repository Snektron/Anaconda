#ifndef SRC_AST_EXPR_FUNCTIONCALLNODE_H_
#define SRC_AST_EXPR_FUNCTIONCALLNODE_H_

#include <string>
#include "ast/expr/expressionnode.h"
#include "ast/argumentlistnode.h"

class FunctionCallNode : public ExpressionNode
{
    private:
        std::string function_var;
        ArgumentListNode* arguments;
        DataTypeBase* called_type;
    public:
        FunctionCallNode(const std::string&, ArgumentListNode*);
        virtual ~FunctionCallNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual DataTypeBase* getType();
        virtual void declareLocals(BrainfuckWriter&);
};

#endif
