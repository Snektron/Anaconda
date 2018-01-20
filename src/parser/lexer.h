#ifndef SRC_PARSER_LEXER_H_
#define SRC_PARSER_LEXER_H_

#include <istream>
#include <optional>
#include "parser/token.h"

class Lexer
{
    private:
        std::istream& input;
        std::size_t row, col;
        std::string buffer;

    public:
        Lexer(std::istream& input);
        Token next();

    private:
        int peek()
        {
            return this->input.peek();
        }

        int consume();
        bool eat(int c);
        bool eatRange(int low, int high);

        void consumeline();

        TokenType nextType();
        Token toToken(Span span, TokenType type);
};

#endif
