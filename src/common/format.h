#ifndef SRC_COMMON_FORMAT_H_
#define SRC_COMMON_FORMAT_H_

#include <string>
#include <memory>
#include <ostream>
#include <sstream>

namespace fmt
{
    template <typename T, typename... Args>
    constexpr void ssprintf(std::ostream& ss, T&& first, Args&&... args)
    {
        if constexpr (sizeof...(Args) == 0)
            ss << first;
        else
        {
            ss << first;
            ssprintf(ss, args...);
        }
    }

    template <typename T, typename... Args>
    std::string sprintf(T first, Args&&... args)
    {
        std::stringstream ss;
        ssprintf(ss, first, args...);
        return ss.str();
    }
}

#endif
