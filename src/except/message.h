#ifndef SRC_EXCEPT_MESSAGE_H_
#define SRC_EXCEPT_MESSAGE_H_

#include <string>

enum class MessageType
{
	WARNING,
	ERROR
};

class Message
{
	private:
		const MessageType type;
		const std::string msg;

	public:
		Message(const MessageType type, const std::string& msg):
			type(type), msg(msg)
		{}

		MessageType getType()
		{
			return type;
		}

		const std::string& getMsg()
		{
			return msg;
		}
};

#endif
