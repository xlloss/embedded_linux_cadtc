#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>

#define DEVFILE "/dev/simple-bad-keys"

unsigned char read_flg;

void sigroutine(int signo)
{
    switch (signo) {
        case SIGALRM:
            //printf("Catch a signal -- SIGALRM \n");
            signal(SIGALRM, sigroutine);
            alarm(1);
            read_flg = 1;
            break;
    }

    return;
}

int main(void)
{
    int fd, ret;
    char ch;
    unsigned char bad_key_value;
    unsigned char key_0, key_1, key_2, key_3;

    read_flg = 0;
    bad_key_value = 0;

    fd = open(DEVFILE, O_RDWR);
    if (fd == -1)
        perror("open");

    alarm(1);
    signal(SIGALRM, sigroutine);

    while (1) {
        if (read_flg) {
            bad_key_value = 0;
            read_flg = 0;
            key_0 = key_1 = key_2 = key_3 = 0;

            ret = read(fd, &bad_key_value, 1);
            if (ret < 0) {
                printf("read key fail ret %d\r\n", ret);
                break;
            }

            switch (bad_key_value) {
            case 0x01:
                key_0 = 1;
                printf("key_0 pushed\n");
                break;

            case 0x02:
                key_1 = 1;
                //printf("key_1 pushed\n");
                break;

            case 0x04:
                key_2 = 1;
                //printf("key_2 pushed\n");
                break;

            case 0x08:
                key_3 = 1;
                //printf("key_3 pushed\n");
                break;

            default:
                break;
            };
        }

        usleep(1);
    };

    if (close(fd) != 0)
        perror("close");

    return 0;
}
