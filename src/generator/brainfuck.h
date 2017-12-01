#ifndef BRAINFUCK_HPP_INCLUDED
#define BRAINFUCK_HPP_INCLUDED

#include <iosfwd>

#include "ast/datatype.h"

class BrainfuckWriter
{
	private:
		std::ostream& output;
	public:
		BrainfuckWriter(std::ostream&);
		~BrainfuckWriter();

	    void declareFunction(const std::string&, 
};

#endif
