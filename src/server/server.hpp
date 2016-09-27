#ifndef __SERVER__
#define __SERVER__

#include <vector>
#include <thread>
#include <mutex>

#include "connection.hpp"
#include "message_handler.hpp"


class Server : public Connection {
public:
    Server(const int port);
    ~Server();
    
    void start();
    
private:
    void main_thread();
    void wait_for_connect();
    void heartbeat();
    void message_handler();
    void connection_cleanup(const uint32_t& index);
    void connection_cleanup();
    

    MessageHandler _msg_handler;
    std::vector<Connection *> _conns;
    std::vector<std::thread> _threads;
    std::mutex _conn_mutex;
    bool _running;
};



#endif
