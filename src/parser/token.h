#ifndef SRC_PARSER_TOKEN_H_
#define SRC_PARSER_TOKEN_H_

#include <string>
#include <utility>

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

    union
    {
        // Only contains data where token text is variable
        // WHITESPACE, IDENT, COMMENT, UNKNOWN
        // see hasText
        std::string text;
    };

    Token(const TokenType type);
    Token(const TokenType type, const std::string& text);
    Token(const Token& other);
    ~Token();

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
