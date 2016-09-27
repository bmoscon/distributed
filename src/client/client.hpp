#ifndef __CLIENT__
#define __CLIENT__


#include <unistd.h>
#include <iostream>

#include "message.hpp"
#include "connection.hpp"
#include "message_types.hpp"
#include "heartbeat_factory.hpp"
#include "message_handler.hpp"
#include "resource_factory.hpp"

class Client : public Connection{

public:
    Client(const std::string &address, const int port) : Connection(address, port)
    {
	_msg_handler.registerHandler(M_HEARTBEAT, HeartbeatFactory::getInstance());
	_msg_handler.registerHandler(M_RESOURCES, ResourceFactory::getInstance());
    }
    
    ~Client()
    {
	Connection::close();
    }
    
    
    
    
    void run() {
	bool connected;
	
	while (true) {
	    do {
		connected = connect();
		if (!connected) {
		    perror("Error: ");
		    std::cout << "Trying again..." << std::endl;
		    sleep(5);
		}
	    } while(!connected);
	    std::cout << "Connected" << std::endl;
	    
	    while (good()) {
		Message r;
		int size = read(r);
		if (size == 0) {
		    std::cout << "Server closed connection" << std::endl;
		    continue;
		}

		_msg_handler.process(r, NULL, this);

		if (r.getType() == M_HEARTBEAT) {
		    Message h(M_HEARTBEAT);
		    write(h);
		}
	    }
	}
    }
    
    
private:
    bool connect()
    {
	int rc;
	
	rc = ::connect(Connection::_sock, (struct sockaddr *) &(Connection::_addr), 
		       sizeof(Connection::_addr));
	if (rc < 0) {
	    // log error from errno
	    return (false);
	}
	
	return (true);
    }


    MessageHandler _msg_handler;
};


#endif
