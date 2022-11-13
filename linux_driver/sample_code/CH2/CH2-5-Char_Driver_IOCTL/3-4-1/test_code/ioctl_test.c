#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include "../ioctl_test.h"

#define DEVFILE "/dev/ioctl"

int main(void)
{
    struct ioctl_arg cmd;
    int fd;
    int ret;
    int code = 0;

    memset(&cmd, 0, sizeof(struct ioctl_arg));
    fd = open(DEVFILE, O_RDWR);
    if (fd < 0)
        perror("open");

    // call IOCTL_VALRESET
    printf("User Level call IOCTL_VALRESET\n");
    ret = ioctl(fd, IOCTL_VALRESET);
    if (ret < 0) {
        printf("errno %d\n", errno);
        perror("ioctl");
    }

    // call IOCTL_VALGET
    printf("User Level call IOCTL_VALGET\n");
        ret = ioctl(fd, IOCTL_VALGET, &cmd);

    if (ret == -1) {
        printf("errno %d\n", errno);
        perror("ioctl");
    }

    printf("User Level val = [%d]\n", cmd.val);

    // call IOCTL_VALSET
    printf("User Level call IOCTL_VALSET\n");
    cmd.val++;
    ret = ioctl(fd, IOCTL_VALSET, &cmd);
    if (ret == -1) {
        printf("errno %d\n", errno);
        perror("ioctl");
    }

    // call IOCTL_VALGET_INT
    cmd.val = 0;
    printf("User Level call IOCTL_VALGET_INT\n");
    ret = ioctl(fd, IOCTL_VALGET_INT, &(cmd.val));
    if (ret == -1) {
        printf("errno %d\n", errno);
        perror("ioctl");
    }
    printf("User Level val = [%d]\n", cmd.val);

    //call IOCTL_VALSET_INT
    printf("User Level call IOCTL_VALSET_INT\n");
    cmd.val++;
    ret = ioctl(fd, IOCTL_VALSET_INT, cmd.val);
    if (ret == -1) {
        printf("errno %d\n", errno);
        perror("ioctl");
    }

    if (close(fd) != 0) {
        perror("close");
    }

    return 0;
}
