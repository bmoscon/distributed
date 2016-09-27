#include "server.hpp"


int main()
{
    Server server(4444);   
    server.start();
    
    return (0);
}
