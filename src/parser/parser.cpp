#include "parser/parser.h"

Parser::Parser(const std::string& input):
    input(input), len(input.length()), state {0, 0, 0, 0} {}

bool Parser::isAtEnd()
{
    return this->state.pos >= this->len;
}

char Parser::peek()
{
    return this->state.pos >= this->len ? EOF : this->input[this->state.pos];
}

char Parser::consume()
{
    int c = peek();
    if (this->state.pos < this->len)
    {
        this->state.pos++;

        if (c == '\n')
        {
            this->state.row++;
            this->state.column = 0;
            this->state.lineStart = this->state.pos;
        }
        else
            this->state.column++;
    }

    return c;
}

Parser::State Parser::save()
{
    return this->state;
}

void Parser::restore(State state)
{
    this->state = state;
}

std::string Parser::capture(State start)
{
    State end = save();

    size_t length = end.pos - start.pos;
    std::string capture(length, '\0');

    restore(start);
    for (size_t i = 0; i < length; i++)
        capture[i] = consume();
    restore(end);

    return capture;
}

std::string Parser::captureLine(State start)
{
    State state = save();

    std::string capture;

    this->state.pos = start.lineStart;
    while (peek() != '\n')
        capture.append(1, (char) consume());

    restore(state);

    return capture;
}

bool Parser::expect(char c)
{
    if (peek() == c)
    {
        consume();
        return true;
    }

    return false;
}

bool Parser::expect(std::string seq)
{
    State state = save();

    for (char ch : seq)
    {
        if (!expect(ch))
        {
            restore(state);
            return false;
        }
    }

    return true;
}

bool Parser::expectRange(char start, char end)
{
    char c = peek();
    if (start <= c && c <= end)
    {
        consume();
        return true;
    }
    return false;
}

bool Parser::whitespace()
{
    if (peek() != ' ' && peek() != '\n' && peek() != '\t' && peek() != '\r')
        return false;
    while (peek() == ' ' || peek() == '\n' || peek() == '\t' || peek() == '\r')
        consume();
    return true;
}

bool Parser::expectUpper()
{
    return expectRange('A', 'Z');
}

bool Parser::expectLower()
{
    return expectRange('a', 'z');
}

bool Parser::expectLetter()
{
    return expectUpper() || expectLower() || expect('_');
}

bool Parser::expectNumber()
{
    return expectRange('0', '9');
}
