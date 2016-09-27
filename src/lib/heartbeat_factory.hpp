#ifndef __HEARTBEAT_FACTORY__
#define __HEARTBEAT_FACTORY__

#include "msg_factory.hpp"
#include "msg_handler.hpp"
#include "heartbeat_handler.hpp"

class HeartbeatFactory : public MsgFactory
{
public:
    static MsgFactory* getInstance()
    {
        static HeartbeatFactory instance;
        
        return (&instance);
    }
    virtual MsgHandler* getHandler()
    {
        return new HeartbeatHandler();
    }
    
    virtual ~HeartbeatFactory() {}
    
private:
    HeartbeatFactory(HeartbeatFactory const&);
    void operator=(HeartbeatFactory const&);
    HeartbeatFactory() {}
};



#endif
