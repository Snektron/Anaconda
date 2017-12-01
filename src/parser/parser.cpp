#include "parser/parser.h"
#include <iostream>

Parser::Parser(const char *source):
	source_(source), current_(0) {}

Node* Parser::parse()
{
	Node *result = file();
	if (!result)
	{
		int c = peek();

		if (c == EOF)
			std::cerr << "Unexpected <EOF>" << std::endl;
		else
			std::cerr << "Error near" << c << std::endl;
	}

	return result;
}

int Parser::peek()
{
	return source_[current_];
}

void Parser::consume()
{
	current_++;
}

bool Parser::expect(int c)
{
	if (peek() == c) {
		consume();
		return true;
	}

	return false;
}

bool Parser::expect(const char *text)
{
	size_t pos = save();
	for (const char *i = text; *i; i++) {
		if (peek() != *i) {
			restore(pos);
			return false;
		} else {
			consume();
		}
	}

	return true;
}

bool Parser::whitespace() {
	bool found = false;

	while(true) {
		int c = peek();
		if (c == ' ' || c == '\t' || c == '\n')
			return found;
		consume();
		found = false;
	}
}

size_t Parser::save()
{
	return current_;
}

void Parser::restore(size_t pos)
{
	current_ = pos;
}

Node* Parser::file()
{
	return nullptr;
}

Node* Parser::statement()
{
	Node *node = ifstat();
	if (node)
		return node;

	return nullptr;
}

Node* Parser::ifstat()
{
	whitespace();
	if (!(expect("if") && !whitespace()))
		return nullptr;

	return nullptr;
}

Node* Parser::funcdecl()
{
	whitespace();
	if (!(expect("func") && !whitespace()))
		return nullptr;

	if (!whitespace())
		return nullptr;



	return nullptr;
}

Node* Parser::unary()
{
	return number();
}

Node* Parser::number()
{
	int c = peek();

	if (!std::isdigit(c))
		return nullptr;

	int value = c - '0';
	consume();

	while (true) {
		c = peek();
		if (!std::isdigit(c))
			break;
		consume();
		value *= 10;
		value += c - '0';
	}

	return nullptr;
}

std::optional<std::string> Parser::name()
{
	int c = peek();
	if (!(std::isalpha(c) || c == '_'))
		return nullptr;

	std::string str(1, c);
	consume();

	while(true) {
		c = peek();
		if (!(std::isalnum(c) || c == '_'))
			break;
		consume();
		str.append(1, c);
	}

	return std::optional<std::string>(str);
}
