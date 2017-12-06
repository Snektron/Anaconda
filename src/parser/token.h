#ifndef SRC_PARSER_TOKEN_H_
#define SRC_PARSER_TOKEN_H_

#include <string>
#include <variant>
#include <vector>
#include <ostream>
#include "types/datatype.h"

enum class TokenType
{
    EOI,
    UNKNOWN,
    WHITESPACE,
    NEWLINE,

    IDENT,

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

    COMMENT
};

enum class Keyword
{
    IF,
    ELSE,
    WHILE,
    TYPE,
    FUNC,
    RETURN
};

enum class BuiltinDataType
{
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
    static const std::vector<const char*> keywords;
    static const std::vector<const char*> builtinDataTypes;

    Span span;

    TokenType type;

    // Contains a string if the type is WHITESPACE, IDENT, COMMENT or UNKNOWN
    // see hasText
    std::variant<std::string> data;

    Token(const TokenType type);
    Token(const TokenType type, const std::string& text);

    std::string asText() const;
    DataTypeBase* asDataType() const;

    bool hasText() const;

    bool isKeyword() const;
    bool isBuiltinDataType() const;
    bool isDataType() const;
    bool isReserved() const;

    template <Keyword T>
    bool isKeyword() const
    {
        return isType<TokenType::IDENT>() && asText() == keywords[(std::size_t) T];
    }

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
std::ostream& operator<<(std::ostream& os, const Keyword kw);
std::ostream& operator<<(std::ostream& os, const Token& token);

#endif
