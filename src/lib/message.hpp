#ifndef __MESSAGE__
#define __MESSAGE__

#include <stdint.h>
#include <cstring>
#include <string>

#include "message_types.hpp"


class Message {
public:
    Message() : type(M_INVALID), payload_size(0), msg(NULL) {}
    Message(uint32_t t) : type(t), payload_size(0), msg(NULL) {}
    Message(const void *data, const size_t size);
    Message(const Message& m);
    ~Message();
    
    Message& operator= (const Message& m);

    uint32_t getType() const;
    uint32_t getPayloadSize() const;
    bool setMsg(const void *data, size_t size);
    std::pair<uint8_t*, size_t> serialize();
    std::string toString() const;
    
    
private:
    uint32_t  type;
    uint32_t  payload_size;
    uint8_t  *msg;
};


#endif
