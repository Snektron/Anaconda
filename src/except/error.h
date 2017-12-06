#ifndef SRC_EXCEPT_ERROR_H_
#define SRC_EXCEPT_ERROR_H_

#include <string>

class Error
{
	private:
		const std::string msg;

	public:
		Error(const std::string& msg):
			msg(msg)
		{}

		const std::string& getMsg()
		{
			return msg;
		}
};

#endif
