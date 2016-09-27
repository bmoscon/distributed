#ifndef __MSG_HANDLER__
#define __MSG_HANDLER__

#include "message.hpp"
#include "message_types.hpp"

class MsgHandler
{
public:
    virtual void processMsg(const Message& m, Connection *src, Connection *rcv) = 0;

};



#endif
