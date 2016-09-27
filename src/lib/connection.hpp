#ifndef __CONNECTION__
#define __CONNECTION__

#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <netinet/in.h>

#include "message.hpp"


class Connection {
public:
    Connection(const int port);
    Connection(const std::string &address, const int port);
    Connection(const struct sockaddr_in addr, int sock) : _addr(addr), _sock(sock), _htimestamp(0){}
    ~Connection();
 
    void close();
    int write(Message& m);
    int read(Message& m);
    int fd() const;
    void heartbeat();
    bool good() const;

protected:
    struct sockaddr_in _addr;
    int _sock;
    time_t _htimestamp;
  
};


#endif
