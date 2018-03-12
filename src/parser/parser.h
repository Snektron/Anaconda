#ifndef SRC_FORMULA_PARSER_H_
#define SRC_FORMULA_PARSER_H_

#include <istream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include "parser/lexer.h"
#include "ast/argumentlistnode.h"
#include "ast/fieldlistnode.h"
#include "ast/global/globalnode.h"
#include "ast/global/globalelementnode.h"
#include "ast/global/globalexpressionnode.h"
#include "ast/global/structuredefinitionnode.h"
#include "ast/global/functiondeclaration.h"
#include "ast/stat/statementnode.h"
#include "ast/stat/returnnode.h"
#include "ast/stat/whilenode.h"
#include "ast/expr/expressionnode.h"
#include "ast/expr/assemblynode.h"

class SyntaxError: public std::runtime_error
{
    public:
        SyntaxError(const Span& span, const std::string& msg);
};

class Parser
{
    private:
        Lexer lexer;
        Token token;

    public:
        Parser(std::istream& input);
        std::unique_ptr<GlobalNode> program();

    private:
        void error(const std::string& msg);

        template <typename T>
        void expected(const T& expected)
        {
            this->error(fmt::sprintf("unexpected token '", this->token, "', expected '", expected, "'"));
        }

        const Token& consume();

        template <TokenType T>
        bool check()
        {
            return this->token.isType<T>();
        }

        template <TokenType T>
        void expect()
        {
            if (!this->check<T>())
                this->expected(T);
            this->consume();
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

        std::unique_ptr<GlobalNode> prog();
        std::unique_ptr<GlobalElementNode> globalstat();
        std::unique_ptr<GlobalExpressionNode> globalexpr();

        std::unique_ptr<StructureDefinitionNode> structdecl();
        std::unique_ptr<FunctionDeclaration> funcdecl();
        std::unique_ptr<FieldListNode> funcpar();
        std::unique_ptr<FieldListNode> fieldlist();

        std::unique_ptr<BlockNode> block();

        std::unique_ptr<StatementNode> statement();
        std::unique_ptr<ReturnNode> returnstat();
        std::unique_ptr<StatementNode> exprstat();
        std::unique_ptr<StatementNode> ifstat();
        std::unique_ptr<WhileNode> whilestat();

        std::unique_ptr<ExpressionNode> expr();
        std::unique_ptr<ExpressionNode> cast();
        std::unique_ptr<ExpressionNode> bor();
        std::unique_ptr<ExpressionNode> bxor();
        std::unique_ptr<ExpressionNode> band();
        std::unique_ptr<ExpressionNode> shift();
        std::unique_ptr<ExpressionNode> sum();
        std::unique_ptr<ExpressionNode> product();
        std::unique_ptr<ExpressionNode> unary();
        std::unique_ptr<ExpressionNode> atom();
        std::unique_ptr<ExpressionNode> paren();
        std::unique_ptr<ArgumentListNode> funcargs();
        std::unique_ptr<ArgumentListNode> arglist();
        std::unique_ptr<ExpressionNode> variable();
        std::unique_ptr<ExpressionNode> rvalue();
        std::unique_ptr<ExpressionNode> lvalue();
        std::unique_ptr<ExpressionNode> constant();
        std::unique_ptr<AssemblyNode> assembly();
        std::string brainfuck();
        std::unique_ptr<DataTypeBase> datatype();
        std::string ident();
        std::unique_ptr<ExpressionNode> toBinOp(
            TokenType type,
            std::unique_ptr<ExpressionNode> lhs,
            std::unique_ptr<ExpressionNode> rhs);
};

#endif
