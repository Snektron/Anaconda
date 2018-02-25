#ifndef SRC_COMMON_UTIL_H_
#define SRC_COMMON_UTIL_H_

#include <vector>
#include <ostream>

#define UNUSED(x) ((void)(x))

template <typename T>
constexpr std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
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

#endif
