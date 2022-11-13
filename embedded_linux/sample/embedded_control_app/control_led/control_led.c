#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <getopt.h>
#include <sys/types.h>
#include <unistd.h>

#define LED_SYS_DEV "/sys/class/leds"
#define LED_DEV "user-led"
#define LED_BRIGHTNESS "brightness"

struct sys_led {
    int dev;
    char *led_buf;
    char *led_path;
    char on[2];
};

int main(int argc, char** argv)
{
    int fd;
    int device = -1;
    int opts;
    int option_index = 0;
    char led_dev_path[50];
    char led_con_buf[50];
    int ret;
    struct sys_led con_led;
    int led_on;

    printf("Led Test v0.1\r\n");

    /* getopt_long stores the option index here. */
    struct option long_options[] = {
        {"device", required_argument, 0, 'd'},
        {"on",  no_argument, 0, 'n'},
        {"off",  no_argument, 0, 'f'},
        {"?",  no_argument, 0, '?'},
        {0, 0, 0, 0}
    };

    while (1) {
        opts = getopt_long (argc, argv,
        "d:nf", long_options, &option_index);

        /* Detect the end of the options. */
        if (opts == -1)
            break;

        switch (opts) {
        case 'd':
            device = atoi(optarg);
            break;

        case 'n':
            led_on = 1;
            break;

        case 'f':
            led_on = 0;
            break;

        case '?':
            printf("help: control_led -d dev_num -on\r\n");
            printf("help: control_led -d dev_num -off\r\n");
            break;

        default:
            break;
        }
    }

    if (device < 0) {
        printf("help: control_led -d dev_num -on\r\n");
        printf("help: control_led -d dev_num -off\r\n");
        return 0;
    }

    con_led.dev = device;
    con_led.led_buf = &led_con_buf[0];

    /* LED_SYS_DEV : /sys/class/leds/     */
    /* LED_DEV     : user-led             */
    /* device      : 1,2                  */
    sprintf(led_dev_path, "%s/%s%d/%s",
        LED_SYS_DEV, LED_DEV, device, LED_BRIGHTNESS);

    con_led.led_path = led_dev_path;

    /* led on or off */
    sprintf(con_led.on, "%d\n", led_on);

    printf("led_dev_path %s\n", led_dev_path);
    printf("con_led.on %s\n", con_led.on);

    ret = access(led_dev_path, R_OK | W_OK);
    if (ret)
        printf("access led_dev_path fail\r");

    fd = open(led_dev_path, O_WRONLY);
    if (fd < 0) {
        printf("open %s fail\r\n", led_dev_path);
        return 0;
    }

    ret = write(fd, con_led.on, 2);
    if (2 != ret) {
        printf("Couldn't set led brightness, ret %d", ret);
        return 0;
    }

    close(fd);
    return 0;
}
