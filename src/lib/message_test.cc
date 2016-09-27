#include "message.hpp"

#include <iostream>

int main()
{
  char buffer[] = "Hello\0";
  Message a(1);
  a.setMsg(buffer, strlen(buffer));
  std::cout << a.toString() << std::endl;
}
