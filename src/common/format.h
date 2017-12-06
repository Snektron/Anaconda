#ifndef SRC_COMMON_FORMAT_H_
#define SRC_COMMON_FORMAT_H_

#include <string>
#include <memory>
#include <sstream>

namespace fmt
{
	namespace
	{
		template <typename T, typename... Args>
		void ssprintf(std::stringstream& ss, T&& first, Args&&... args)
		{
			if constexpr (sizeof...(Args) == 0)
				ss << first;
			else
			{
				ss << first;
				ssprintf(ss, args...);
			}
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
