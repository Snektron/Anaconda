#include "parser/lexer.h"

Lexer::Lexer(std::istream& input):
    input(input) {}

Token Lexer::next()
{
    switch (consume())
    {
        case EOF:
            return Token(TokenType::EOI);
        case '{':
            return Token(TokenType::BRACE_OPEN);
        case '}':
            consume();
            return Token(TokenType::BRACE_CLOSE);
        case '(':
            return Token(TokenType::PAREN_OPEN);
        case ')':
            return Token(TokenType::PAREN_CLOSE);
        case '-':
            if (peek() == '>')
            {
                consume();
                return Token(TokenType::ARROW);
            }

            return Token(TokenType::MINUS);
        case ',':
            return Token(TokenType::COMMA);
        case '=':
            return Token(TokenType::EQUALS);
        case '+':
            return Token(TokenType::PLUS);
        case '*':
            return Token(TokenType::STAR);
        case '/':
            if (peek() == '/')
                return Token(TokenType::COMMENT, "/" + readline());
            return Token(TokenType::SLASH);
        case '%':
            return Token(TokenType::PERCENT);
        case ';':
            return Token(TokenType::SEMICOLON);
        case '\n':
            return Token(TokenType::NEWLINE);
    }

    if (std::optional<Token> tok = whitespace())
        return tok.value();

    if (std::optional<Token> tok = literals())
           return tok.value();

    if (std::optional<Token> tok = name())
           return tok.value();

    return Token(TokenType::UNKNOWN, std::string(1, peek()));
}

std::string Lexer::readline()
{
    std::string text;

    int c;
    while ((c = peek()) != '\n')
        text.append(1, (int) c);

    return text;
}

std::optional<Token> Lexer::whitespace()
{
    std::string text;

    int c = peek();
    while (c == ' ' || c == '\t' || c == '\r')
    {
        text.append(1, (char) c);
        c = peek();
    }

    if (text.empty())
        return std::nullopt;

    return std::make_optional(Token(TokenType::WHITESPACE, text));
}

std::optional<Token> Lexer::name()
{
    int c = peek();
    if ((c < 'a' || c > 'z') &&
        (c < 'A' || c > 'Z') &&
        (c != '_'))
    {
        return std::nullopt;
    }

    std::string text;

    do
    {
        text.append(1, c);
        c = peek();
    }
    while (
        (c > 'a' && c < 'z') ||
        (c > 'A' && c < 'Z') ||
        (c > '0' && c < '9') ||
        (c == '_'));

    return std::nullopt;
}

std::optional<Token> Lexer::literals()
{
    return std::nullopt;
}
