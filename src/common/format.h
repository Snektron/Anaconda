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
    constexpr void fprintf(std::ostream& os, Args&&... args)
    {
        (os << ... << args);
    }

    template <typename... Args>
    constexpr std::string sprintf(Args&&... args)
    {
        std::stringstream ss;
        fprintf(ss, args...);
        return ss.str();
    }

    template <typename... Args>
    constexpr void printf(Args&&... args)
    {
        fprintf(std::cout, std::forward<Args>(args)...);
    }
}

#endif
