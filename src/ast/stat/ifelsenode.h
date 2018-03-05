#ifndef SRC_AST_STAT_IFELSENODE_H_
#define SRC_AST_STAT_IFELSENODE_H_

#include "ast/stat/statementnode.h"
#include "ast/expr/expressionnode.h"

class IfElseNode : public StatementNode
{
    private:
        ExpressionNode* conditional;
        StatementNode* statement;
        StatementNode* else_statement;
    public:
        IfElseNode(ExpressionNode*, StatementNode*, StatementNode*);
        virtual ~IfElseNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual void declareLocals(BrainfuckWriter&);
};

#endif
