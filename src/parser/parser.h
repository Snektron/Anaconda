#ifndef SRC_PARSER_PARSER_H_
#define SRC_PARSER_PARSER_H_

#include <istream>
#include <string>
#include "ast/node.h"

class Parser {
	const char *source_;
	size_t current_;

public:
	Parser(const char *source);
	Node* parse();
private:
	int peek();
	void consume();
	bool expect(int c);
	size_t save();
	void restore(size_t pos);

	Node* file();
};

#endif
