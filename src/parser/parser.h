#ifndef SRC_PARSER_PARSER_H_
#define SRC_PARSER_PARSER_H_

#include <istream>

class Node;

class Parser {
	std::istream &input_;
public:
	Parser(std::istream &input);
};

#endif
