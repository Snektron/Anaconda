#ifndef SRC_PARSER_TOKEN_H_
#define SRC_PARSER_TOKEN_H_

#include <string>
#include <optional>
#include <vector>
#include <ostream>
#include "types/datatype.h"

enum class TokenType
{
    EOI,
    UNKNOWN,
    WHITESPACE,
    NEWLINE,
    COMMENT,

    IDENT,
    INTEGER,

    BRACE_OPEN,
    BRACE_CLOSE,
    PAREN_OPEN,
    PAREN_CLOSE,

    ARROW,
    COMMA,
    EQUALS,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    PERCENT,
    SEMICOLON,

    IF,
    ELSE,
    WHILE,
    TYPE,
    FUNC,
    RETURN,

    U8,
    VOID
};

struct Span
{
    std::size_t row, col;
};

struct Token
{
    static const std::vector<const char*> types;

    Span span;

    TokenType type;

    // Contains a string if the type is WHITESPACE, IDENT, COMMENT, INTEGER or UNKNOWN
    // see hasText
    std::string lexeme;

    Token(const Span span, const TokenType type);
    Token(const Span span, const TokenType type, const std::string& text);

    bool isKeyword() const;
    bool isBuiltinDataType() const;
    bool isReserved() const;
    bool isDataType() const;

    bool hasText() const;

    DataTypeBase* asDataType() const;

    template <TokenType T>
    bool isType() const
    {
        return this->type == T;
    }

    template <TokenType T, TokenType... Args>
    bool isOneOf() const
    {
        if constexpr (sizeof...(Args) == 0)
            return isType<T>();
        else
            return isType<T>() || isOneOf<Args...>();
    }
};

std::ostream& operator<<(std::ostream& os, const TokenType type);
std::ostream& operator<<(std::ostream& os, const Token& token);

#endif
