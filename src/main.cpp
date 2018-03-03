#include <iostream>
#include <fstream>
#include <memory>
#include "parser/parser.h"
#include "ast/node.h"
#include "common/util.h"

void parse(const char* name)
{
    std::ifstream file(name);

    if (!file)
    {
        std::cerr << "Error: failed to open '" << name << std::endl;
        return;
    }

    std::cout << "Parsing " << name << std::endl;

    Parser p(file);

    std::unique_ptr<GlobalNode> root = p.program();
    root->print(std::cout, 0);
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input>" << std::endl;
        return 0;
    }

    parse(argv[1]);

    return 0;
}
