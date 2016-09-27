#include <cassert>
#include <ctime>
#include <cstring>
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>

#include "connection.hpp"
#include "util.hpp"



#define HEARTBEAT_TIMEOUT 90
#define MAGIC_NUMBER 0x4444


Connection::Connection(const int port)
{
    _htimestamp = 0;
    _addr = {0};
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(_sock > 0);
}


Connection::Connection(const std::string &address, const int port)
{
    _htimestamp = 0;
    _addr = {0};
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    inet_aton(address.c_str(), &_addr.sin_addr);
    
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(_sock > 0);
}


Connection::~Connection()
{
    close();
}


void Connection::close()
{
    if (_sock > 0) {
	::close(_sock);
	_sock = -1;
    }
}

int Connection::write(Message& m)
{
    uint8_t header[8];
    int ret;
    std::pair<uint8_t *, size_t> msg = m.serialize();

    ((uint32_t *)header)[0] = msg.second;
    ((uint32_t *)header)[1] = MAGIC_NUMBER;

    ret = sock_write(_sock, header, sizeof(header));
    if (ret < 1) {
	if (ret == 0) {
	    close();
	}
	return (ret);
    }
    
    ret = sock_write(_sock, msg.first, msg.second);
    if (ret == 0) {
	close();
    }
    
    return (ret);
}

int Connection::read(Message& m)
{
    uint8_t header[8];
    uint8_t *buffer;
    size_t size;
    int ret;
    
    ret = sock_read(_sock, header, sizeof(header));
    if (ret < 1) {
	if (ret == 0) {
	    close();
	}
	return (ret);
    }
    
    assert(((uint32_t *)header)[1] == MAGIC_NUMBER);
    size = ((uint32_t *)header)[0];
    
    buffer = new uint8_t[size];

    ret = sock_read(_sock, buffer, size);
    if (ret < 1) {
	if (ret == 0) {
	    close();
	}
	return (0);
    }
    
    m = Message(buffer, size);
    
    delete[] buffer;
    return (ret);
}


int Connection::fd() const
{
    return (_sock);
}


void Connection::heartbeat()
{
    _htimestamp = time(NULL);
}


bool Connection::good() const
{
    return (_htimestamp == 0 || ((_htimestamp > (time(NULL) - HEARTBEAT_TIMEOUT)) && _sock > 0));
}
    
