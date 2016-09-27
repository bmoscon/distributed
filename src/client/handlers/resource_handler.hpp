#ifndef __RESOURCE_HANDLER__
#define __RESOURCE_HANDLER__

#include <fstream>
#include <limits>
#include <sys/sysinfo.h>

#include "message.hpp"
#include "msg_handler.hpp"

static uint64_t get_mem_total()
{
    std::string token;
    std::ifstream file("/proc/meminfo");
    while(file >> token) {
        if(token == "MemTotal:") {
            uint64_t mem;
            if(file >> mem) {
                return mem;
            } else {
                return (0);       
            }
        }
       
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return (0);
}

class ResourceHandler : public MsgHandler
{
public:
    void processMsg(const Message& m, Connection *src, Connection *rcv)
    {
	std::string mem = std::to_string(get_mem_total());
	Message rsp(M_RESOURCES);
	rsp.setMsg(mem.c_str(), mem.length());
	rcv->write(rsp);

	
    }
    
    
};


#endif
