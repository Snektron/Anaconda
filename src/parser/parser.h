#ifndef SRC_FORMULA_PARSER_H_
#define SRC_FORMULA_PARSER_H_

#include <istream>
#include <vector>
#include <string>
#include "parser/lexer.h"
#include "ast/node.h"

#include <iostream>

class Parser
{
    private:
        Lexer lexer;
        Token token;

        std::string message;
        std::vector<TokenType> tried;

    public:
        Parser(std::istream& input);
        GlobalNode* program();

        const std::string& getMessage() const
        {
            return this->message;
        }

    private:
        void error(const std::string& msg);
        void unexpected();
        const Token& consume();

        template <TokenType T>
        bool check()
        {
            if (this->token.isType<T>())
                return true;
            this->tried.push_back(T);
            return false;
        }

        template <TokenType T>
        bool expect()
        {
            if (this->check<T>()) {
                this->consume();
                return true;
            }

            unexpected();
            return false;
        }

        template <TokenType T>
        bool eat()
        {
            if (this->check<T>())
            {
                this->consume();
                return true;
            }

            return false;
        }

        template <TokenType H, TokenType... T>
        bool eatOneOf()
        {
            if constexpr (sizeof...(T) == 0)
                return this->eat<H>();
            else
                return this->eat<H>() || this->eatOneOf<T...>();
        }

        GlobalNode* prog();
        GlobalElementNode* globalstat();
        GlobalExpressionNode* globalexpr();

        StructureDefinitionNode* structdecl();
        FunctionDeclaration* funcdecl();
        FieldListNode* funcpar();
        FieldListNode* fieldlist();

        BlockNode* block();

        StatementNode* statlist();
        StatementNode* statement();
        ReturnNode* returnstat();
        StatementNode* exprstat();
        StatementNode* ifstat();
        WhileNode* whilestat();

        ExpressionNode* expr();
        ExpressionNode* bor();
        ExpressionNode* bxor();
        ExpressionNode* band();
        ExpressionNode* shift();
        ExpressionNode* sum();
        ExpressionNode* product();
        ExpressionNode* unary();
        ExpressionNode* assignment();
        ExpressionNode* lvalue();
        ExpressionNode* atom();
        ExpressionNode* paren();
        ArgumentListNode* funcargs();
        ArgumentListNode* arglist();
        ExpressionNode* variable();
        ExpressionNode* constant();
        AssemblyNode* assembly();
        std::string* brainfuck();
};

#endif
