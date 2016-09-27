#ifndef __HEARTBEAT_HANDLER__
#define __HEARTBEAT_HANDLER__

#include "message.hpp"
#include "msg_handler.hpp"

class HeartbeatHandler : public MsgHandler
{
public:
    void processMsg(const Message& m, Connection *src, Connection *rcv)
    {
	rcv->heartbeat();
    }
    
    
};


#endif
