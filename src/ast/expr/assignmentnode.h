#ifndef SRC_AST_EXPR_ASSIGNMENTNODE_H_
#define SRC_AST_EXPR_ASSIGNMENTNODE_H_

#include "ast/expr/expressionnode.h"

class AssignmentNode : public ExpressionNode
{
    private:
        ExpressionNode* lop;
        ExpressionNode* rop;
    public:
        AssignmentNode(ExpressionNode*, ExpressionNode*);
        virtual ~AssignmentNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual DataTypeBase* getType();
        virtual void declareLocals(BrainfuckWriter&);
};

#endif
