#ifndef SRC_FORMULA_PARSER_H_
#define SRC_FORMULA_PARSER_H_

#include <istream>
#include "parser/lexer.h"
#include "ast/node.h"

class Parser
{
    protected:
        Lexer lexer;
        Token token;

    public:
        Parser(std::istream& input);
        GlobalNode* program();

    protected:
        void unexpected();

        Token nextFiltered();
        const Token& consume();

        bool isAtEnd()
        {
            return this->token.isType<TokenType::EOI>();
        }

        template <TokenType T>
        bool expect()
        {
            if (this->token.isType<T>())
            {
                consume();
                return true;
            }

            unexpected();
            return false;
        }

        template <Keyword T>
        bool expect()
        {
            if (this->token.isKeyword<T>())
            {
                consume();
                return true;
            }

            unexpected();
            return false;
        }

        template <TokenType T>
        bool check()
        {
            if (this->token.isType<T>())
            {
                return true;
            }

            return false;
        }

        template <Keyword T>
        bool check()
        {
            if (this->token.isKeyword<T>())
            {
                return true;
            }

            return false;
        }

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

#endif /* SRC_FORMULA_PARSER_H_ */
