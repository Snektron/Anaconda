#ifndef SRC_AST_GLOBAL_GLOBALNODE_H_
#define SRC_AST_GLOBAL_GLOBALNODE_H_

#include "ast/global/globalelementnode.h"

class GlobalNode : public Node
{
    private:
        std::vector<GlobalElementNode*> elements;
    public:
        GlobalNode(const std::vector<GlobalElementNode*>&);
        virtual ~GlobalNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareGlobals(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
};

#endif
