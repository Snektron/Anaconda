#include "parser/lexer.h"

Lexer::Lexer(std::istream& input):
    input(input), row(0), col(0) {}

int Lexer::consume()
{
    int c = this->input.get();
    if (c != EOF)
    {
        if (c == '\n')
        {
            this->col = 0;
            this->row++;
        }
        else
            this->col++;
    }

    return c;
}

Token Lexer::next()
{
    Span span{this->row, this->col};
    Token tok = this->next0();
    tok.span = span;
    return tok;
}

std::string Lexer::readline()
{
    std::string text;

    int c;
    while ((c = peek()) != '\n')
    {
        text.append(1, (char) this->consume());
        c = this->peek();
    }

    return text;
}

Token Lexer::next0()
{
    if (std::optional<Token> tok = singleByteTok())
        return tok.value();

    if (std::optional<Token> tok = multiByteTok())
        return tok.value();

    if (std::optional<Token> tok = whitespace())
        return tok.value();

    if (std::optional<Token> tok = literals())
           return tok.value();

    if (std::optional<Token> tok = name())
           return tok.value();

    return Token(TokenType::UNKNOWN, std::string(1, peek()));
}

std::optional<Token> Lexer::singleByteTok()
{
    switch (this->peek())
    {
        case EOF:
            this->consume();
            return std::make_optional<Token>(TokenType::EOI);
        case '{':
            this->consume();
            return std::make_optional<Token>(TokenType::BRACE_OPEN);
        case '}':
            this->consume();
            return std::make_optional<Token>(TokenType::BRACE_CLOSE);
        case '(':
            this->consume();
            return std::make_optional<Token>(TokenType::PAREN_OPEN);
        case ')':
            this->consume();
            return std::make_optional<Token>(TokenType::PAREN_CLOSE);
        case ',':
            this->consume();
            return std::make_optional<Token>(TokenType::COMMA);
        case '=':
            this->consume();
            return std::make_optional<Token>(TokenType::EQUALS);
        case '+':
            this->consume();
            return std::make_optional<Token>(TokenType::PLUS);
        case '*':
            this->consume();
            return std::make_optional<Token>(TokenType::STAR);
        case '%':
            this->consume();
            return std::make_optional<Token>(TokenType::PERCENT);
        case ';':
            this->consume();
            return std::make_optional<Token>(TokenType::SEMICOLON);
        case '\n':
            this->consume();
            return std::make_optional<Token>(TokenType::NEWLINE);
        default:
            return std::nullopt;
    }
}

std::optional<Token> Lexer::multiByteTok()
{
    switch (this->peek())
    {
        case '/':
           if (this->peek() == '/')
           {
               this->consume();
               return std::make_optional<Token>(TokenType::COMMENT, "/" + readline());
           }
           return std::make_optional<Token>(TokenType::SLASH);
        case '-':
            if (this->peek() == '>')
            {
                this->consume();
                return std::make_optional<Token>(TokenType::ARROW);
            }
            return std::make_optional<Token>(TokenType::MINUS);
        default:
            return std::nullopt;
    }
}

std::optional<Token> Lexer::whitespace()
{
    std::string text;

    int c = this->peek();
    while (c == ' ' || c == '\t' || c == '\r')
    {
        text.append(1, (char) this->consume());
        c = this->peek();
    }

    if (text.empty())
        return std::nullopt;

    return std::make_optional<Token>(TokenType::WHITESPACE, text);
}

std::optional<Token> Lexer::name()
{
    int c = this->peek();
    if ((c < 'a' || c > 'z') &&
        (c < 'A' || c > 'Z') &&
        (c != '_'))
    {
        return std::nullopt;
    }

    std::string text;

    do
    {
        text.append(1, (char) this->consume());
        c = this->peek();
    }
    while (
        (c > 'a' && c < 'z') ||
        (c > 'A' && c < 'Z') ||
        (c > '0' && c < '9') ||
        (c == '_'));

    return std::make_optional<Token>(TokenType::IDENT, text);
}

std::optional<Token> Lexer::literals()
{
    return std::nullopt;
}
