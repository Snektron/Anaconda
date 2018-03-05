#ifndef SRC_AST_STAT_WHILENODE_H_
#define SRC_AST_STAT_WHILENODE_H_

#include "ast/stat/statementnode.h"
#include "ast/expr/expressionnode.h"

class WhileNode : public StatementNode
{
    private:
        ExpressionNode* conditional;
        StatementNode* statement;
    public:
        WhileNode(ExpressionNode*, StatementNode*);
        virtual ~WhileNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual void declareLocals(BrainfuckWriter&);
};

#endif
