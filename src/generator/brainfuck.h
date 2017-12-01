#ifndef BRAINFUCK_HPP_INCLUDED
#define BRAINFUCK_HPP_INCLUDED

#include <stack>

class BrainfuckWriter
{
	private:
		std::ostream& output;
		std::stack<std::stack<std::map<std::string, size_t>>> stacks;
	public:
		BrainfuckWriter(std::ostream&);
		~BrainfuckWriter();

		void start_scope();
		void end_scope();
		void new_scope();

		
};

#endif
