#ifndef SRC_AST_NODE_H_
#define SRC_AST_NODE_H_

#include <iosfwd>
#include <string>
#include <vector>
#include <map>
#include "types/datatype.h"

class BrainfuckWriter;
class DataTypeBase;

class Node
{
    protected:
        void printIndent(std::ostream&, size_t) const;
    public:
        virtual ~Node() = default;

        virtual void print(std::ostream&, size_t) const = 0;
        virtual void generate(BrainfuckWriter&) = 0;
        virtual void declareGlobals(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&) = 0;
};

#endif
