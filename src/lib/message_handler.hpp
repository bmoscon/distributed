#ifndef __MESSAGE_HANDLER__
#define __MESSAGE_HANDLER__

#include <unordered_map>
#include <stdint.h>

#include "message.hpp"
#include "message_types.hpp"
#include "msg_factory.hpp"
#include "msg_handler.hpp"

class MessageHandler
{
public:
    MessageHandler() {}
    
    void registerHandler(msg_types t, MsgFactory *fac)
    {
	std::unordered_map<uint32_t, MsgFactory *>::iterator it;
	std::pair<std::unordered_map<uint32_t, MsgFactory *>::iterator, bool> r;
	
	it = factory_map.find(t);
	if (it == factory_map.end()) {
	    r = factory_map.insert(std::make_pair(t, fac));
	    if (!r.second) {
		// error
	    }
	}
    }
    
    /* Processes a generic Message
     *
     * m   - the message to be processed 
     * src - the Connection source of the message
     * rcv - the Connection that received the message
     */
    void process(Message& m, Connection *src, Connection *rcv)
    {
	MsgHandler *h = factory_map[m.getType()]->getHandler();
	h->processMsg(m, src, rcv);
    }
    
private:
    std::unordered_map<uint32_t, MsgFactory *> factory_map;
    
};



#endif
