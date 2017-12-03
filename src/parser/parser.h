#ifndef SRC_FORMULA_PARSER_H_
#define SRC_FORMULA_PARSER_H_

#include <string>

class Parser
{
	public:
		struct State {
			std::size_t pos;
			std::size_t row, column;
			std::size_t lineStart;
		};

	protected:
		std::string input;
		std::size_t len;

		State state;

	public:
		Parser(const std::string& input);

	protected:
		bool isAtEnd();

		char peek();
		char consume();

		State save();
		void restore(State state);
		std::string capture(State start);
		std::string captureLine(State start);

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
