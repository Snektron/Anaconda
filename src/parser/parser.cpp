#include "parser/parser.h"

Parser::Parser(const std::string& input):
	input(input), pos(0),
	len(input.length()), line(0) {}

std::size_t Parser::getLine()
{
	return this->line;
}

bool Parser::isAtEnd()
{
	return this->pos >= this->len;
}

char Parser::peek()
{
	return this->pos >= this->len ? EOF : this->input[this->pos];
}

char Parser::consume()
{
	int c = peek();
	if (this->pos < this->len)
	{
		if (c == '\n')
			this->line++;
		this->pos++;
	}

	return c;
}

state_t Parser::save()
{
	return this->pos;
}

void Parser::restore(state_t pos)
{
	this->pos = pos;
}

void Parser::beginCapture()
{
	this->capturestack.push(this->pos);
}

std::string Parser::endCapture()
{
	if (!this->capturestack.empty())
	{
		state_t start = this->capturestack.top(), end = this->pos;
		this->capturestack.pop();
		size_t length = end - start;
		std::string capture(length, '\0');

		state_t state = save();
		this->pos = start;
		for (size_t i = 0; i < length; i++)
			capture[i] = consume();
		restore(state);

		return capture;
	}

	return "";
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
	state_t state = save();

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

bool Parser::whitespace() {
	if (peek() != ' ' && peek() != '\n' && peek() != '\t')
		return false;
	while(peek() == ' ' || peek() == '\n' || peek() == '\t')
		consume();
	return true;
}

bool Parser::expectUpper() {
	return expectRange('A', 'Z');
}

bool Parser::expectLower() {
	return expectRange('a', 'z');
}

bool Parser::expectLetter() {
	return expectUpper() || expectLower();
}

bool Parser::expectNumber() {
	return expectRange('0', '9');
}
