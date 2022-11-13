#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVFILE "/dev/chrdev_sys_info"
#define SYSFS "/sys/class/chrdev_sys/chrdev_sys_info/id"

int main(void)
{
    int fd, readn, i;
    FILE *fd_sys;
    char read_buf[20] = {0};

    fd = open(DEVFILE, O_RDWR);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    sleep(3);

    close(fd);

    fd_sys = fopen(SYSFS, "r");
    if (fd_sys == NULL) {
        perror("fd_sys open");
        return -1;
    }

    readn = fread(read_buf, sizeof(char), 10, fd_sys);
    printf("read_buf %s\r\n", read_buf);
    fclose(fd_sys);

    return 0;
}
