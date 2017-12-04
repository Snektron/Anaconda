#ifndef SRC_PARSER_TOKEN_H_
#define SRC_PARSER_TOKEN_H_

#include <string>
#include <utility>
#include <variant>

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

    U8,
    VOID
};

struct Token
{
    const TokenType type;

    // Contains a string if the type is WHITESPACE, IDENT, COMMENT or UNKNOWN
    // see hasText
    std::variant<std::string> data;

    Token(const TokenType type);
    Token(const TokenType type, const std::string& text);

    bool isKeyword(Keyword kw) const;
    bool hasText() const;

    template <TokenType T>
    bool isType() const
    {
        return this->type == T;
    }

    template <TokenType T, TokenType... Args>
    bool isOneOf() const
    {
        if constexpr (sizeof...(Args) == 1)
            return isType<T>();
        else
            return isType<T>() || isOneOf<Args...>();
    }
};

#endif
