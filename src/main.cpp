#include <iostream>
#include <fstream>
#include <memory>
#include "parser/parser.h"
#include "ast/node.h"
#include "common/util.h"
#include "common/format.h"

void parse(const char* name)
{
    std::ifstream file(name);

    if (!file)
    {
        fmt::fprintf(std::cerr, "Error: failed to open '", name, "'\n");
        return;
    }

    fmt::printf("Parsing ", name, '\n');

    Parser p(file);

    try
    {
        std::unique_ptr<GlobalNode> root = p.program();
        root->print(std::cout, 0);
    }
    catch (const SyntaxError& err)
    {
        fmt::fprintf(std::cerr, err.what());
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fmt::fprintf(std::cerr, "Usage: ", argv[0], " <input>\n");
        return 0;
    }

    parse(argv[1]);

    return 0;
}
