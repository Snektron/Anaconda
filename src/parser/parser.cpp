#include "parser/parser.h"

Parser::Parser(const char *source):
	source_(source), current_(0) {}

Node* Parser::parse() {
	return file();
}

int Parser::peek()
{
	return source_[current_];
}

void Parser::consume()
{
	current_++;
}

bool Parser::expect(int c) {
	if (peek() == c) {
		consume();
		return true;
	}

	return false;
}

size_t Parser::save() {
	return current_;
}

void Parser::restore(size_t pos) {
	current_ = pos;
}

Node* Parser::file() {
	return nullptr;
}


