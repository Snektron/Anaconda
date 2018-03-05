#ifndef SRC_AST_ARGUMENTLISTNODE_H_
#define SRC_AST_ARGUMENTLISTNODE_H_

#include "ast/node.h"
#include "ast/expr/expressionnode.h"

class ArgumentListNode : public Node
{
    private:
        std::vector<ExpressionNode*> arguments;
    public:
        ArgumentListNode(const std::vector<ExpressionNode*>& arguments);
        virtual ~ArgumentListNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareGlobals(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);

        std::vector<DataTypeBase*> getArgumentTypes();
};

#endif
