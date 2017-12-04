#include "parser/token.h"

const char* keywords[] =
{
    "if", "else", "while", "type", "func",
    "u8", "void"
};

Token::Token(const TokenType type):
    type(type) {}

Token::Token(const TokenType type, const std::string& text):
    type(type), data(text) {}

bool Token::isKeyword(Keyword kw) const
{
    return isType<TokenType::IDENT>() && std::get<std::string>(this->data) == keywords[(int) kw];
}

bool Token::hasText() const
{
    return isOneOf<TokenType::WHITESPACE, TokenType::IDENT, TokenType::COMMENT>();
}
