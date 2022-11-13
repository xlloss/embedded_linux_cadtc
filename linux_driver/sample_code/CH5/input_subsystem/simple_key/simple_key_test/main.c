#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>


#define TEST_KEY_CODE KEY_BLUE
//#define TEST_KEY_CODE BTN_0

int main(void)
{
    char *filename="/dev/input/event2";
    struct input_event ev;
    int fd, code;

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "failed to open input device %s: %s\n",
			filename, strerror(errno));
        return -1;
    }

    while (1) {
        if (read(fd, &ev, sizeof(struct input_event)) < 0) {
            fprintf(stderr,
				"failed to read input event from input device %s: %s\n",
				filename, strerror(errno));
        }

        if (ev.type == EV_KEY) {
	    if ( ev.code == TEST_KEY_CODE)
		printf("get key code:0x%x value:%d\n", ev.code, ev.value);
	}
    };
    close(fd);
    return 0;
}

