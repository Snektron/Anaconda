#include "parser/token.h"

#include <iostream>

const std::vector<const char*> Token::types =
{
    "<eoi>", "<unknown>", "<whitespace>", "<newline>", "<comment>",
    "<ident>", "<integer>",
    "{", "}", "(", ")", "[", "]",
    "->", ",", ".", "=", "+", "-", "*", "/", "%", ";", "<", ">", "<=", ">=",
    "&", "|", "<<", ">>", "^",
    "if", "else", "while", "type", "func", "return", "asm",
    "u8", "void"
};

Token::Token(const Span span, const TokenType type):
    span(span), type(type) {}

Token::Token(const Span span, const TokenType type, const Token::Lexeme&& lexeme):
    span(span), type(type), lexeme(std::move(lexeme)) {}

Token::Token(const Token& other):
    span(other.span), type(other.type), lexeme(other.lexeme) {}

Token& Token::operator=(const Token& other) {
    this->span = other.span;
    this->type = other.type;
    this->lexeme = other.lexeme;
    return *this;
}

bool Token::isKeyword() const
{
    return this->isOneOf<TokenType::IF, TokenType::ELSE, TokenType::WHILE, TokenType::TYPE, TokenType::FUNC, TokenType::RETURN>();
}

bool Token::isBuiltinDataType() const
{
    return this->isOneOf<TokenType::U8, TokenType::VOID>();
}

bool Token::isReserved() const
{
    return this->isKeyword() || this->isBuiltinDataType();
}

bool Token::isDataType() const
{
    return this->isBuiltinDataType() || this->isType<TokenType::IDENT>();
}

std::unique_ptr<DataTypeBase> Token::asDataType()
{
    if (!this->isDataType())
        return nullptr;

    switch (this->type) {
        case TokenType::U8:
            return std::make_unique<DataType<DataTypeClass::U8>>();
        case TokenType::VOID:
            return std::make_unique<DataType<DataTypeClass::VOID>>();
        default:
            return std::make_unique<DataType<DataTypeClass::STRUCT_FORWARD>>(this->lexeme.get<std::string>());
    }
}

bool Token::hasText() const
{
    return this->isOneOf<TokenType::WHITESPACE, TokenType::IDENT, TokenType::COMMENT, TokenType::INTEGER>();
}

std::ostream& operator<<(std::ostream& os, const TokenType type)
{
    os << Token::types[(std::size_t) type];
    return os;
}

std::ostream& operator<<(std::ostream& os, const Token& token)
{
    if (token.hasText())
        os << token.lexeme;
    else
        os << token.type;

    return os;
}

std::ostream& operator<<(std::ostream& os, const Span& span)
{
    os << "(" << span.row << ", " << span.col << ")";

    return os;
}