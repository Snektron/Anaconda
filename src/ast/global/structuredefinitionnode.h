#ifndef SRC_AST_GLOBAL_STRUCTUREDEFINITIONNODE_H_
#define SRC_AST_GLOBAL_STRUCTUREDEFINITIONNODE_H_

#include <string>
#include "ast/global/globalelementnode.h"
#include "ast/fieldlistnode.h"

class StructureDefinitionNode : public GlobalElementNode
{
    private:
        std::string name;
        FieldListNode* members;
        DataType<DataTypeClass::STRUCT_FORWARD>* type;
    public:
        StructureDefinitionNode(const std::string&, FieldListNode* members);
        ~StructureDefinitionNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareGlobals(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
};

#endif
