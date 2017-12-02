#ifndef SRC_FORMULA_PARSER_H_
#define SRC_FORMULA_PARSER_H_

#include <string>
#include <stack>

typedef std::size_t state_t;

class Parser {
protected:
	std::string input_;
	state_t pos_, len_;

	std::stack<state_t> capturestack_;

public:
	Parser(const std::string& input);

protected:
	bool atEnd();

	char peek();
	char consume();

	state_t save();
	void restore(state_t backup);

	void beginCapture();
	std::string endCapture();

	bool expect(char c);
	bool expect(std::string seq);
	bool expectRange(char start, char end);

	bool whitespace();

	bool expectUpper();
	bool expectLower();
	bool expectLetter();
	bool expectNumber();
};

#endif /* SRC_FORMULA_PARSER_H_ */
