#ifndef SRC_FORMULA_PARSER_H_
#define SRC_FORMULA_PARSER_H_

#include <string>
#include <stack>

typedef std::size_t state_t;

class Parser
{
	protected:
		std::string input;
		state_t pos;
		std::size_t len, line;
		std::stack<state_t> capturestack;

	public:
		Parser(const std::string& input);

	protected:
		std::size_t getLine();

		bool isAtEnd();

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
