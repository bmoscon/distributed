#ifndef __RESOURCE_HANDLER__
#define __RESOURCE_HANDLER__


#include "message.hpp"
#include "msg_handler.hpp"


class ResourceHandler : public MsgHandler
{
public:
    void processMsg(const Message& m, Connection *src, Connection *rcv)
    {
	std::cout << "Client total memory " << m.toString() << std::endl;
    }
    
    
};


#endif
