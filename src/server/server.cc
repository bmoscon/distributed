#include <iostream>
#include <cassert>
#include <unistd.h>

#include "server.hpp"
#include "connection.hpp"
#include "message.hpp"
#include "message_types.hpp"
#include "heartbeat_factory.hpp"
#include "resource_factory.hpp"


#define HEARTBEAT_INTERVAL 5


Server::Server(const int port) : Connection(port)
{
    _running = false;
    assert(Connection::_sock > 0);
    
    assert(bind(Connection::_sock, (struct sockaddr *) &(Connection::_addr), 
		sizeof(Connection::_addr)) >= 0);
    
    _msg_handler.registerHandler(M_HEARTBEAT, HeartbeatFactory::getInstance());
    _msg_handler.registerHandler(M_RESOURCES, ResourceFactory::getInstance());
}


Server::~Server()
{
    _running = false;
    for (size_t i = 0; i < _conns.size(); ++i) {
	delete _conns[i];
    }

    for (size_t i = 0; i < _threads.size(); ++i) {
	_threads[i].join();
    }
}


void Server::start()
{
    _running = true;
    
    _threads.push_back(std::thread(&Server::main_thread, this));
}


void Server::main_thread()
{   
    // init heartbeat thread and message processing thread
    _threads.push_back(std::thread(&Server::heartbeat, this));
    _threads.push_back(std::thread(&Server::message_handler, this));
    
    while (_running) {
	wait_for_connect();
	
    }
}


void Server::wait_for_connect()
{
    int conn_fd;
    
    assert(listen(Connection::_sock, 1024) >= 0);
    socklen_t len;
    struct sockaddr_in client;
    do {
	len = sizeof(client);
	if ((conn_fd = accept(Connection::_sock, (struct sockaddr *) &client, &len)) < 0) {
	    if ((errno != EPROTO) && (errno != ECONNABORTED)) {
		assert(false);
	    }
	}
    } while (errno == EPROTO || errno == ECONNABORTED);
    
    _conn_mutex.lock();
    _conns.push_back(new Connection(client, conn_fd));
    _conn_mutex.unlock();
}


void Server::heartbeat()
{
    Message m(M_HEARTBEAT);
    
    while (_running) {
	sleep(HEARTBEAT_INTERVAL);

	connection_cleanup();

	_conn_mutex.lock();
	for (size_t i = 0; i < _conns.size(); ++i) {
	    int rc = _conns[i]->write(m);
	    sleep(2);
	    Message r(M_RESOURCES);
	    _conns[i]->write(r);
	    if (rc == -1) {
		perror("Write failed");
	    } else if (rc == 0) {
		// connection closed
		connection_cleanup(i);
	    }
	}
	_conn_mutex.unlock();
    } 
}

void Server::message_handler()
{
    while (_running) {
	fd_set fds;
	int maxfd;
	struct timeval timeout = { 5, 0 };
	
	if (_conns.size() == 0) {
	    sleep(1);
	    continue;
	}

	FD_ZERO(&fds);
	for (uint32_t i = 0; i < _conns.size(); ++i) {
	    FD_SET(_conns[i]->fd(), &fds);
	    maxfd = std::max(maxfd, _conns[i]->fd());
	}
	
	if (select(maxfd+1, &fds, NULL, NULL, &timeout) == -1) {
	    if (errno == EINTR) {
		continue;
	    }
	    std::cout << "Errno: " << errno << std::endl;
	    assert(0);
	}

	for(uint32_t i = 0; i < _conns.size(); ++i) {
	    if (FD_ISSET(_conns[i]->fd(), &fds)) {
		// got the FD that is ready to be read
		Message m;
		int size = _conns[i]->read(m);
		if (size == 0) {
		    std::cout << "client closed connection" << std::endl;
		    connection_cleanup(i);
		    --i;
		} else if (size == -1) {

		} else {
		    _msg_handler.process(m, _conns[i], this);
		    if (m.getType() == M_HEARTBEAT) {
			struct tm *tm = localtime(&_htimestamp);
			char date[20];
			strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm);
			std::cout << "Heartbeat last recvd: " << date << std::endl;
		    }
		}
	    }
	}
	
    }
}

void Server::connection_cleanup(const uint32_t& index)
{
    _conn_mutex.lock();
    _conns[index]->close();
    delete _conns[index];
    _conns.erase(_conns.begin() + index);
    _conn_mutex.unlock();
}

void Server::connection_cleanup()
{
    _conn_mutex.lock();
    for (uint32_t i = 0; i < _conns.size(); ++i) {
	if (!_conns[i]->good()) {
	    _conns[i]->close();
	    delete _conns[i];
	    _conns.erase(_conns.begin() + i);
	    --i;
	}
    }
    _conn_mutex.unlock();
}
