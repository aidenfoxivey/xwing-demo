#include "randombytes.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

int randombytes(uint8_t *output, size_t n)
{
    if (!output && n > 0) {
        return -1;  // Invalid parameters
    }
    
    if (n == 0) {
        return 0;  // Nothing to do
    }
    
    // Use /dev/urandom as a fallback random source
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) {
        return -1;
    }
    
    ssize_t bytes_read = 0;
    while (bytes_read < (ssize_t)n) {
        ssize_t result = read(fd, output + bytes_read, n - bytes_read);
        if (result < 0) {
            close(fd);
            return -1;
        }
        bytes_read += result;
    }
    
    close(fd);
    return 0;  // Success
}