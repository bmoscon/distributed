#ifndef __UTIL__
#define __UTIL__


int sock_write(int fd, const uint8_t *buffer, const size_t len);
int sock_read(int fd, uint8_t *buffer, const size_t len);

#endif
