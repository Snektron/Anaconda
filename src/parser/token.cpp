#include "parser/token.h"

const std::vector<const char*> Token::types =
{
    "<eoi>", "<unknown>", "<whitespace>", "<newline>", "<comment>"
    "<ident>", "<int>", "<literal>"
    "(", ")", "{", "}",
    "->", ",", "=", "+", "-", "*", "/", "%", ";",
    "if", "else", "while", "type", "func", "return",
    "u8", "void",
    "<comment>"
};

Token::Token(const Span span, const TokenType type):
    span(span), type(type) {}

Token::Token(const Span span, const TokenType type, const Token::Lexeme&& lexeme):
    span(span), type(type), lexeme(std::move(lexeme)) {}

bool Token::isKeyword() const
{
    return isOneOf<TokenType::IF, TokenType::ELSE, TokenType::WHILE, TokenType::TYPE, TokenType::FUNC, TokenType::RETURN>();
}

bool Token::isBuiltinDataType() const
{
    return isOneOf<TokenType::U8, TokenType::VOID>();
}

bool Token::isReserved() const
{
    return isKeyword() || isBuiltinDataType();
}

bool Token::isDataType() const
{
    return isBuiltinDataType() || isType<TokenType::IDENT>();
}

DataTypeBase* Token::asDataType()
{
    if (!this->isDataType())
        return nullptr;

    switch (this->type) {
        case TokenType::U8:
            return new DataType<DataTypeClass::U8>();
        case TokenType::VOID:
            return new DataType<DataTypeClass::VOID>();
        default:
            return new DataType<DataTypeClass::STRUCT_FORWARD>(this->lexeme.get<std::string>());
    }
}

bool Token::hasText() const
{
    return isOneOf<TokenType::WHITESPACE, TokenType::IDENT, TokenType::COMMENT, TokenType::INTEGER>();
}

std::ostream& operator<<(std::ostream& os, const TokenType type)
{
    os << Token::types[(std::size_t) type];
    return os;
}

std::ostream& operator<<(std::ostream& os, const Token& token)
{
    if (token.hasText())
        os << '\'' << token.lexeme << '\'';
    else
        os << token.type;

    return os;
}
