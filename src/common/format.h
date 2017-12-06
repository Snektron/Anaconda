#ifndef SRC_COMMON_FORMAT_H_
#define SRC_COMMON_FORMAT_H_

#include <string>
#include <memory>
#include <ostream>
#include <sstream>
#include <vector>

namespace fmt
{
    template <typename T>
    std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
    {
        bool first(true);
        for (const T& item : vec)
        {
            if (!first)
                os << ", ";
            else
                first = false;

            os << item;
        }
        return os;
    }

    template <typename T, typename... Args>
    void ssprintf(std::ostream& ss, T&& first, Args&&... args)
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
