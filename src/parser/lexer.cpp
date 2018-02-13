#include "parser/lexer.h"
#include "common/variant.h"

Lexer::Lexer(std::istream& input):
    input(input), row(0), col(0) {}

Token Lexer::next()
{
    this->buffer.clear();

    Span span{this->row, this->col};
    TokenType type = this->nextType();

    return toToken(span, type);
}

int Lexer::consume()
{
    int c = this->input.get();
    this->buffer.append(1, c);

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

bool Lexer::eat(int c)
{
    if (this->peek() == c)
    {
        this->consume();
        return false;
    }

    return true;
}

bool Lexer::eatRange(int low, int high)
{
    if (this->peek() >= low && this->peek() <= high)
    {
        this->consume();
        return false;
    }

    return true;
}

void Lexer::consumeline()
{
    while (peek() != '\n')
        this->consume();
}

TokenType Lexer::nextType()
{
    switch (this->consume())
    {
        case EOF: return TokenType::EOI;
        case '{': return TokenType::BRACE_OPEN;
        case '}': return TokenType::BRACE_CLOSE;
        case '(': return TokenType::PAREN_OPEN;
        case ')': return TokenType::PAREN_CLOSE;
        case ',': return TokenType::COMMA;
        case '=': return TokenType::EQUALS;
        case '+': return TokenType::PLUS;
        case '*': return TokenType::STAR;
        case '%': return TokenType::PERCENT;
        case ';': return TokenType::SEMICOLON;
        case '\n': return TokenType::NEWLINE;
        case '/':
            if (this->eat('/'))
            {
                this->consumeline();
                return TokenType::COMMENT;
            }
            return TokenType::SLASH;
        case '-': return this->eat('>') ? TokenType::ARROW : TokenType::MINUS;
        case ' ':
        case '\t':
        case '\r':
            while (this->eat(' ') || this->eat('\t') || this->eat('\r'))
                continue;
            return TokenType::WHITESPACE;
        case 'a' ... 'z':
        case 'A' ... 'Z':
        case '_':
            while(this->eatRange('a', 'z'), this->eatRange('A', 'Z'), this->eatRange('0', '9'), this->eat('_'))
                continue;
            return TokenType::IDENT;
        case '0' ... '9':
            while(this->eatRange('0', '9'))
                continue;
            return TokenType::INTEGER;
        default:
            return TokenType::UNKNOWN;
    }    
}

Token Lexer::toToken(Span span, TokenType type)
{
    switch (type)
    {
        case TokenType::IDENT:
            // Too lazy for perfect hash
            if (this->buffer == "if")
                return Token(span, TokenType::IF);
            else if (this->buffer == "else")
                return Token(span, TokenType::ELSE);
            else if (this->buffer == "while")
                return Token(span, TokenType::WHILE);
            else if (this->buffer == "type")
                return Token(span, TokenType::TYPE);
            else if (this->buffer == "func")
                return Token(span, TokenType::FUNC);
            else if (this->buffer == "return")
                return Token(span, TokenType::RETURN);
            else
                return Token(span, TokenType::IDENT, Token::Lexeme::make<std::string>(std::move(buffer)));
        case TokenType::WHITESPACE:
        case TokenType::COMMENT:
        case TokenType::UNKNOWN:
            return Token(span, type, Token::Lexeme::make<std::string>(std::move(buffer)));
        default:
            return Token(span, type);
    }
}