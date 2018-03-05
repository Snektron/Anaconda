#ifndef SRC_AST_GLOBAL_FUNCTIONDECLARATION_H_
#define SRC_AST_GLOBAL_FUNCTIONDECLARATION_H_

#include "ast/fieldlistnode.h"
#include "ast/global/globalelementnode.h"
#include "ast/stat/blocknode.h"

class FunctionDeclaration : public GlobalElementNode
{
    private:
        std::string name;
        FieldListNode* parameters;
        DataTypeBase* return_type;
        BlockNode* content;
        size_t scope;
    public:
        FunctionDeclaration(const std::string&, FieldListNode*, DataTypeBase*, BlockNode*);
        virtual ~FunctionDeclaration();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareGlobals(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
};

#endif
