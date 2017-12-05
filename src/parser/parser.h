#ifndef SRC_FORMULA_PARSER_H_
#define SRC_FORMULA_PARSER_H_

#include <istream>
#include <vector>
#include <string>
#include "parser/lexer.h"
#include "ast/node.h"

class Parser
{
    private:
        Lexer lexer;
        Token token;

        std::vector<std::string> warnings;
        std::vector<std::string> errors;

    public:
        Parser(std::istream& input);
        GlobalNode* program();

        const std::vector<std::string>& getWarnings() const
        {
            return this->warnings;
        }

        const std::vector<std::string>& getErrors() const
        {
            return this->errors;
        }

    private:
        void error();
        void warn();
        void unexpected();

        Token nextFiltered();
        const Token& consume();

        template <TokenType... T>
        bool expect()
        {
            if (this->token.isOneOf<T...>())
                return true;

            unexpected();
            return false;
        }

        template <Keyword... T>
        bool expect()
		{
			if (this->token.isKeyword<T...>())
				return true;

			unexpected();
			return false;
		}

        template <TokenType... T>
        bool eat()
        {
        	if (this->token.isOneOf<T...>())
        	{
        		consume();
        		return true;
        	}

        	return false;
        }

        template <Keyword T>
		bool eat()
		{
			if (this->token.isKeyword<T>())
			{
				consume();
				return true;
			}

			return false;
		}

        GlobalNode* prog();
        GlobalElementNode* globalstat();
        GlobalExpressionNode* globalexpr();

        StructureDefinitionNode* structdecl();

        FunctionDeclaration* funcdecl();
        FunctionParameters* funcpar();

        BlockNode* block();

        StatementListNode* statlist();
        StatementNode* statement();
        ReturnNode* returnstat();
        StatementNode* exprstat();
        StatementNode* ifstat();
        WhileNode* whilestat();

        ExpressionNode* expr();
        ExpressionNode* sum();
        ExpressionNode* product();
        ExpressionNode* unary();
        ExpressionNode* assignment();
        ExpressionNode* lvalue();
        ExpressionNode* atom();
        ExpressionNode* paren();
        FunctionArguments* funcargs();
        VariableNode* variable();
};

#endif /* SRC_FORMULA_PARSER_H_ */
