#include "except/message.h"

std::ostream& operator<<(std::ostream& os, const Message msg) {
    os << msg.getMsg();
    return os;
}
