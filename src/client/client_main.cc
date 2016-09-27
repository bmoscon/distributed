#include "client.hpp"


int main()
{
  Client a("127.0.0.1", 4444);
  
  a.run();
  
  return (0);
}
