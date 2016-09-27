#include <cassert>
#include <iostream>

#include "message.hpp"


Message::Message(const void *data, const size_t size)
{
    assert(size > 0);
    assert(data);
    uint8_t *d = (uint8_t *)data;
    
    type = ((uint32_t *)d)[0];
    payload_size = ((uint32_t *)d)[1];

    assert(size == payload_size + sizeof(type) + sizeof(payload_size));
    
    msg = new uint8_t[size];
    
    memcpy(msg, data, size);
}


Message::Message(const Message& m)
{
    type = M_INVALID;
    payload_size = 0;
    msg = NULL;

    *this = m;
    
}

Message::~Message()
{
    if (msg) {
	delete[] msg;
    }
}


Message& Message::operator= (const Message& m)
{
    if (msg) {
	delete[] msg;
    }

    type = m.type;
    payload_size = m.payload_size;
    msg = new uint8_t[sizeof(type) + sizeof(payload_size) + payload_size];
    
    memcpy(msg, m.msg, sizeof(type) + sizeof(payload_size) + payload_size);

    return (*this);
}


uint32_t Message::getType() const
{
    return (type);
}

uint32_t Message::getPayloadSize() const
{
    return (payload_size);
}



bool Message::setMsg(const void *data, size_t size)
{
    if (msg) {
	delete[] msg;
    }

    payload_size = size;
    msg = new uint8_t[size + sizeof(type) + sizeof(payload_size)];
    
    ((uint32_t *)msg)[0] = type;
    ((uint32_t *)msg)[1] = payload_size;
    
    if (data) {
	memcpy(msg + sizeof(type) + sizeof(payload_size), data, payload_size);
    }

    return (true);
}


std::pair<uint8_t *, size_t> Message::serialize()
{
    uint32_t msg_size = sizeof(type) + sizeof(payload_size) + payload_size;

    if ((!msg) && (!payload_size)) {
	// you dont have to have a payload
	assert(setMsg(NULL, 0));
    }
    
    return (std::make_pair(msg, msg_size));
}

std::string Message::toString() const
{
    int offset = sizeof(type) + sizeof(payload_size);
    
    return std::string((char *)msg+offset, payload_size);
}
