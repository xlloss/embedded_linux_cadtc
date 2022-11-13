#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVFILE "/dev/chrdev_alloc"

int main(void)
{
    int fd;
	int test_data;

    fd = open(DEVFILE, O_RDWR);
    if (fd == -1)
        perror("open");

    sleep(3);

	write(fd, &test_data, 1);

    if (close(fd) != 0)
        perror("close");

    return 0;
}
