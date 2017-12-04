#include "parser/token.h"

const char* keywords[] =
{
    "if", "else", "while", "type", "func",
    "u8", "void"
};

Token::Token(const TokenType type):
    type(type) {}

Token::Token(const TokenType type, const std::string& text):
    type(type), text(text) {}

Token::Token(const Token& other):
    type(other.type)
{
    if (other.hasText())
        this->text = other.text;
}

Token::~Token()
{
    if (hasText())
        this->text.~basic_string();
}

bool Token::isKeyword(Keyword kw) const
{
    return isType<TokenType::IDENT>() && this->text == keywords[(int) kw];
}

bool Token::hasText() const
{
    return isOneOf<TokenType::WHITESPACE, TokenType::IDENT, TokenType::COMMENT>();
}
