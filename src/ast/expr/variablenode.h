#ifndef SRC_AST_EXPR_VARIABLENODE_H_
#define SRC_AST_EXPR_VARIABLENODE_H_

#include <string>
#include "ast/expr/expressionnode.h"

class VariableNode : public ExpressionNode
{
    private:
        std::string variable;
        DataTypeBase* datatype;
    public:
        VariableNode(const std::string&);
        virtual ~VariableNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual DataTypeBase* getType();
        virtual void declareLocals(BrainfuckWriter&);

        std::string getName();
};

#endif
