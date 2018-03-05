#ifndef SRC_AST_FIELDLISTNODE_H_
#define SRC_AST_FIELDLISTNODE_H_

#include "ast/node.h"
#include "common/field.h"

class FieldListNode : public Node
{
    private:
        std::vector<Field> arguments;
    public:
        FieldListNode(const std::vector<Field>&);
        virtual ~FieldListNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);

        std::vector<Field>& getParameters();
};

#endif
