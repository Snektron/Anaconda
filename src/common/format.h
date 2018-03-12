#ifndef SRC_COMMON_FORMAT_H_
#define SRC_COMMON_FORMAT_H_

#include <string>
#include <memory>
#include <ostream>
#include <sstream>
#include <iostream>
#include <utility>

namespace fmt
{
    template <typename... Args>
    void fprintf(std::ostream& os, const Args&... args)
    {
        (os << ... << args);
    }

    template <typename... Args>
    std::string sprintf(const Args&... args)
    {
        std::stringstream ss;
        fprintf(ss, args...);
        return ss.str();
    }

    template <typename... Args>
    void printf(const Args&... args)
    {
        fprintf(std::cout, args...);
    }
}

#endif
