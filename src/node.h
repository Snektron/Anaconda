#ifndef NODE_HPP_INCLUDED
#define NODE_HPP_INCLUDED

#include <iosfwd>

class Node
{
	public:
		virtual ~Node();
		
		virtual void print(std::ostream&);
		virtual void generate(std::ostream&);
};

#endif
