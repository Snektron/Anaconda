#include "parser/parser.h"

Parser::Parser(const std::string& input):
	input_(input), pos_(0),
	len_(input.length()), line_(0) {}

std::size_t Parser::line()
{
	return line_;
}

bool Parser::atEnd()
{
	return pos_ >= len_;
}

char Parser::peek()
{
	return pos_ >= len_ ? EOF : input_[pos_];
}

char Parser::consume()
{
	int c = peek();
	if (pos_ < len_)
	{
		if (c == '\n')
			line_++;
		pos_++;
	}

	return c;
}

state_t Parser::save()
{
	return pos_;
}

void Parser::restore(state_t pos)
{
	pos_ = pos;
}

void Parser::beginCapture()
{
	capturestack_.push(pos_);
}

std::string Parser::endCapture()
{
	if (!capturestack_.empty())
	{
		state_t start = capturestack_.top(), end = pos_;
		capturestack_.pop();
		size_t length = end - start;
		std::string capture(length, '\0');

		state_t state = save();
		pos_ = start;
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
