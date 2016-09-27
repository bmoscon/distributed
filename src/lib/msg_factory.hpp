#ifndef __MSG_FACTORY__
#define __MSG_FACTORY__

#include "msg_handler.hpp"

class MsgFactory
{
public:
    virtual MsgHandler *getHandler() = 0;
    virtual ~MsgFactory() {}
};



#endif
