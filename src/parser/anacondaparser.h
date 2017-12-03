#ifndef SRC_PARSER_ANACONDAPARSER_H_
#define SRC_PARSER_ANACONDAPARSER_H_

#include "parser/parser.h"
#include "ast/node.h"

class AnacondaParser : public Parser
{
    public:
        AnacondaParser(const std::string& input);
        GlobalNode* program();
    private:
        std::string id();
        DataTypeBase* type();

        GlobalNode* prog();
        GlobalElementNode* globalstat();

        StructureDefinitionNode* structdecl();

        FunctionDeclaration* funcdecl();
        FunctionParameters* funcpar();

        BlockNode* block();

        StatementListNode* statlist();
        StatementNode* statement();
        StatementNode* ifstat();
        WhileNode* whilestat();
        AssignmentNode* assignstat();
        template <typename T>
        T* declstat();

        ExpressionNode* expr();
        ExpressionNode* sum();
        ExpressionNode* product();
        ExpressionNode* unary();
        ExpressionNode* atom();
        ExpressionNode* paren();
        FunctionCallNode* funccall();
        FunctionArguments* funcargs();
        VariableNode* variable();
};

#endif
