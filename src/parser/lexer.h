#ifndef SRC_PARSER_LEXER_H_
#define SRC_PARSER_LEXER_H_

#include <istream>
#include <optional>
#include "parser/token.h"

class Lexer
{
    private:
        std::istream& input;

    public:
        Lexer(std::istream& input);
        Token next();
    private:
        int peek()
        {
            return this->input.peek();
        }

        int consume()
        {
            return this->input.get();
        }

        std::string readline();

        std::optional<Token> whitespace();
        std::optional<Token> name();
        std::optional<Token> literals();
};

#endif
