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

#define IIO_SYS_DEV "/sys/bus/iio/devices"
#define IIO_DEV "iio:device"
#define IIO_LUM "in_illuminance0_input"

struct sys_iio {
	int dev;
	char *iio_buf;
    char *iio_path;
};

int main(int argc, char** argv)
{
    int fd;
    int port, duty, device = -1, period, start;
    int set_duty = 0, set_period = 0, set_start;
    int opts;
    int option_index = 0;
    char iio_lum_path[50];
    char iio_con_buf[50];
    int i, ret, readbuf_cnt;
    struct sys_iio con_iio_lum;
    char str_lum[10];
    int int_lum;

    size_t count;

    printf("Light Test v0.1\r\n");

    /* getopt_long stores the option index here. */
    struct option long_options[] = {
        {"device", required_argument, 0, 'd'},
        {"?",  no_argument, 0, '?'},
        {0, 0, 0, 0}
    };

    while (1) {
        opts = getopt_long (argc, argv,
        "d:", long_options, &option_index);

        /* Detect the end of the options. */
        if (opts == -1)
            break;

        switch (opts) {
        case 'd':
            device = atoi(optarg);
            break;
        case '?':
            printf("help: control_light -d device_number\r\n");
            break;
        default:
            break;
        }
    }

    if (device < 0) {
        printf("please assign device number\r\n");
        printf("help: control_light -d device_number\r\n");
        return 0;
    }

    con_iio_lum.dev = device;
    con_iio_lum.iio_buf = &iio_con_buf[0];

    sprintf(iio_lum_path, "%s/%s%d/%s", IIO_SYS_DEV, IIO_DEV, device, IIO_LUM);
    con_iio_lum.iio_path = iio_lum_path;
    
    ret = access(iio_lum_path, R_OK | W_OK);
    if (ret)
        printf("access iio_lum_path fail\r");

    fd = open(iio_lum_path, O_ASYNC, S_IRUSR | S_IRGRP | S_IROTH);
    if (fd < 0)
        printf("open %s fail\r\n", iio_lum_path);

    count = 10;
    readbuf_cnt = read(fd, str_lum, count);
    if (readbuf_cnt < count)

    i = 0;
    while (1) {
        if (str_lum[i] == 0x0a) {
            str_lum[i] = 0;
            str_lum[i + 1] = 0;
            break;
        }
        i++;
    }
    int_lum = atoi(str_lum);
    printf("lum %d\r\n", int_lum);
    return 0;
}
