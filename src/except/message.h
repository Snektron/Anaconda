#ifndef SRC_EXCEPT_MESSAGE_H_
#define SRC_EXCEPT_MESSAGE_H_

#include <string>
#include <ostream>

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

        MessageType getType() const
        {
            return type;
        }

        const std::string& getMsg() const
        {
            return msg;
        }
};

std::ostream& operator<<(std::ostream& os, const Message msg);

#endif
