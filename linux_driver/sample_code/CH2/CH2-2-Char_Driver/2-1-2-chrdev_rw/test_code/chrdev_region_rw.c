#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVFILE "/dev/chrdev_rw"

int main(void)
{
    int fd;
    int id = 0;

    fd = open(DEVFILE, O_RDWR);
    if (fd == -1)
        perror("open");

    read(fd, &id, 1);

    printf("User Level Read ID = [%d]\n", id);

    id = id + 5;
    write(fd, &id, 1);

    read(fd, &id, 1);

    printf("User Level Read ID = [%d]\n", id);

    id = 100;
    write(fd, &id, 1);

    read(fd, &id, 1);

    printf("User Level Read ID = [%d]\n", id);


    if (close(fd) != 0)
        perror("close");

    return 0;
}
