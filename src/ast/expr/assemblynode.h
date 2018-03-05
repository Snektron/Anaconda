#ifndef SRC_AST_EXPR_ASSEMBLYNODE_H_
#define SRC_AST_EXPR_ASSEMBLYNODE_H_

#include <string>
#include "ast/expr/expressionnode.h"
#include "ast/argumentlistnode.h"

class AssemblyNode : public ExpressionNode
{
    private:
        DataTypeBase* datatype;
        std::string assembly;
        ArgumentListNode* arguments;
    public:
        AssemblyNode(DataTypeBase*, const std::string&, ArgumentListNode*);
        virtual ~AssemblyNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual DataTypeBase* getType();
        virtual void declareLocals(BrainfuckWriter&);
};

#endif
