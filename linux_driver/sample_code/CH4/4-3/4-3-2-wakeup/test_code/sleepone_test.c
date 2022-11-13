#include <stdio.h>
#include <fcntl.h>

int main ( ) 
{
    int fd;
    char r_buf;

    fd = open("/dev/sleepone", O_RDWR);
    if (fd < 0) {
        printf("Error in opening file \n");
        return -1;
    }

    read(fd, &r_buf, sizeof(r_buf));
    printf("User level Read Data = [%c]\n", r_buf);
    close(fd);
    return 0;
}
