#ifndef SRC_AST_EXPR_DECLARATIONNODE_H_
#define SRC_AST_EXPR_DECLARATIONNODE_H_

#include "ast/expr/expressionnode.h"

class DeclarationNode: public ExpressionNode
{
    private:
        DataTypeBase* datatype;
        std::string variable;
    public:
        DeclarationNode(DataTypeBase*, const std::string&);
        virtual ~DeclarationNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual void declareGlobals(BrainfuckWriter&);
        virtual DataTypeBase* getType();
        virtual void declareLocals(BrainfuckWriter&);
};

#endif
