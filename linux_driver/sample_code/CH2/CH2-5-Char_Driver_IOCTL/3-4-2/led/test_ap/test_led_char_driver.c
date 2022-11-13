#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <getopt.h>
#include <unistd.h>

#define DEVFILE "/dev/leds"
#define LED_NUM 4
#define LED_ON 1
#define LED_OFF 0

int main(int argc, char** argv)
{
    int fd;
    int led, value;
    int opts;
    int option_index = 0;

    /* getopt_long stores the option index here. */
    struct option long_options[] = {
        {"led", required_argument, 0, 'l'},
        {"value", required_argument, 0, 'v'},
        {"?",  no_argument, 0, '?'},
        {0, 0, 0, 0}
    };

    while (1) {
        opts = getopt_long (argc, argv, "l:v:",
        long_options, &option_index);

        /* Detect the end of the options. */
        if (opts == -1)
            break;

        switch (opts) {
        case 'l':
            led = atoi(optarg);
            break;

        case 'v':
            value = atoi(optarg);
            break;

        case '?':
            printf(
                "./test_led_char_driver -l LED_NUM -v ON/OFF\r\n"
                "LED_NUM : 0 - 3\r\n"
                "ON/OFF :\r\n"
                "   ON : 1\r\n"
                "   OFF: 0\r\n"
                "./test_led_char_driver -l 0 -v 1\r\n : led 0 on\r\n"
                "./test_led_char_driver -l 0 -v 0\r\n : led 0 off\r\n");
            break;
        default:
                break;
        }
    }

	fd = open(DEVFILE, O_RDWR);
    if (!fd)
        perror("open");

	ioctl(fd, value, led);

	close(fd);
    return 0;
}
