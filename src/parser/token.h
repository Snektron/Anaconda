#ifndef SRC_PARSER_TOKEN_H_
#define SRC_PARSER_TOKEN_H_

#include <string>
#include <optional>
#include <vector>
#include <ostream>
#include "types/datatype.h"
#include "common/variant.h"

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
    BRACKET_OPEN,
    BRACKET_CLOSE,

    ARROW,
    COMMA,
    DOT,
    EQUALS,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    PERCENT,
    SEMICOLON,
    LEFT,
    RIGHT,
    LEFTEQ,
    RIGHTEQ,

    AMPERSAND,
    PIPE,
    LEFTLEFT,
    RIGHTRIGHT,
    HAT,

    IF,
    ELSE,
    WHILE,
    TYPE,
    FUNC,
    RETURN,
    ASM,

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

    typedef Variant<std::string, uint64_t> Lexeme;

    Span span;

    TokenType type;

    // Contains a string if the type is WHITESPACE, IDENT, COMMENT, INTEGER or UNKNOWN.
    Lexeme lexeme;

    template <typename T, typename... Args>
    static Token make_mv(const Span span, const TokenType type, Args&&... args) {
        return Token(span, type, Lexeme::make_mv<T>(std::forward<Args...>(args...)));
    }

    template <typename T, typename... Args>
    static Token make(const Span span, const TokenType type, Args&... args) {
        return Token(span, type, Lexeme::make<T>(args...));
    }

    Token(const Span span, const TokenType type);
    Token(const Span span, const TokenType type, const Lexeme&& lexeme);

    bool isKeyword() const;
    bool isBuiltinDataType() const;
    bool isReserved() const;
    bool isDataType() const;

    bool hasText() const;

    DataTypeBase* asDataType();

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
