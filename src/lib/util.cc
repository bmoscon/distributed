#include <errno.h>
#include <unistd.h>
#include <stdint.h>

int sock_write(int fd, const uint8_t *buffer, const size_t len)
{
    uint32_t remaining_bytes = len;
    int32_t  write_bytes;
    
    if ((!buffer) || (!len) || (fd < 1)) {
	errno = EINVAL;
	return (-1);
    }
    
    while (remaining_bytes > 0) {
	if ((write_bytes = write(fd, buffer, remaining_bytes)) <= 0) {
	    if ((errno == EINTR) && (write_bytes < 0)) {
		write_bytes = 0;
	    } else {
		return (-1);
	    }
	}
	
	remaining_bytes -= write_bytes;
	buffer += write_bytes;
    }
    
    return (len - remaining_bytes); 
}


int sock_read(int fd, uint8_t *buffer, const size_t len)
{
    int32_t read_bytes = -1;
    
    if ((!buffer) || (!len) || (fd < 1)) {
	errno = EINVAL;
	return (-1);
    }
    
    do {
	if ((read_bytes = read(fd, buffer, len)) < 0) {
	    if (errno != EINTR) {
		return (-1);
	    }
	}
    } while (errno == EINTR);
    
    return (read_bytes);
}
