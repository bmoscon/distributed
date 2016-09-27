#ifndef __RESOURCE_FACTORY__
#define __RESOURCE_FACTORY__

#include "msg_factory.hpp"
#include "msg_handler.hpp"
#include "resource_handler.hpp"


class ResourceFactory : public MsgFactory
{
public:
    static MsgFactory* getInstance()
    {
        static ResourceFactory instance;
        
        return (&instance);
    }
    virtual MsgHandler* getHandler()
    {
        return new ResourceHandler();
    }
    
    virtual ~ResourceFactory() {}
    
private:
    ResourceFactory(ResourceFactory const&);
    void operator=(ResourceFactory const&);
    ResourceFactory() {}
};





#endif
