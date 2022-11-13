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
#include <time.h>

#define HWMON_SYS_DEV "/sys/class/hwmon/"
#define HWMON_DEV "hwmon"
#define HWMON_TEMP "temp1_input"

struct sys_hwmon {
	int dev;
	char *hwmon_buf;
    char *hwmon_path;
};

int main(int argc, char** argv)
{
    int fd;
    int port, duty, device = -1, period, start;
    int set_duty = 0, set_period = 0, set_start;
    int opts;
    int option_index = 0;
    char hwmon_temp_path[50];
    char hwmon_con_buf[50];
    int i, ret, readbuf_cnt;
    struct sys_hwmon con_hwmon_temp;
    char str_temp[10];
    int int_temp;
	FILE *fd_write_file;
	struct tm *timeinfo;
	time_t rawtime;
	char time_buf[100], write_buf[150];
	char log_file[100];
    size_t count;

    printf("Hwmon Temp Test v0.1\r\n");
	time(&rawtime );
	timeinfo = localtime(&rawtime);
	sprintf(time_buf, "Current local time and date: %s", asctime(timeinfo));
	printf("time_buf %s\n", time_buf);

    /* getopt_long stores the option index here. */
    struct option long_options[] = {
        {"device", required_argument, 0, 'd'},
        {"log", required_argument, 0, 'l'},
        {"?",  no_argument, 0, '?'},
        {0, 0, 0, 0}
    };

    while (1) {
        opts = getopt_long (argc, argv,
        "d:l:", long_options, &option_index);

        /* Detect the end of the options. */
        if (opts == -1)
            break;

        switch (opts) {
        case 'd':
            device = atoi(optarg);
            break;
        case 'l':
            sprintf(log_file, "%s", optarg);
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

    con_hwmon_temp.dev = device;
    con_hwmon_temp.hwmon_buf = &hwmon_con_buf[0];

    sprintf(hwmon_temp_path, "%s/%s%d/%s", HWMON_SYS_DEV, HWMON_DEV, device, HWMON_TEMP);
    con_hwmon_temp.hwmon_path = hwmon_temp_path;

    ret = access(hwmon_temp_path, R_OK | W_OK);
    if (ret) {
        printf("access hwmon_temp_path fail\r");
		exit(1);
	}

	count = 10;

	while (1) {
		time (&rawtime );
		timeinfo = localtime ( &rawtime );
		sprintf(time_buf, "Current local time and date: %s\n", asctime(timeinfo));

		fd = open(hwmon_temp_path, O_ASYNC, S_IRUSR | S_IRGRP | S_IROTH);
		if (fd < 0) {
			printf("open %s fail\r\n", hwmon_temp_path);
			exit(1);
		}

		readbuf_cnt = read(fd, str_temp, count);
		if (readbuf_cnt >= count || readbuf_cnt < 0) {
			printf("read temp fail\n");
			exit(1);
		}

		i = 0;
		while (1) {
			if (str_temp[i] == 0x0a) {
				str_temp[i] = 0;
				str_temp[i + 1] = 0;
				break;
			}
			i++;
		}
		int_temp = atoi(str_temp);
		printf("Temp %d\r\n", int_temp);

		sprintf(write_buf, "%s Temp %d\n", time_buf, int_temp);

		fd_write_file = fopen(log_file, "a+");
		if (fd_write_file < 0)
			printf("fopen %s fail\r\n", "/home/root/test_file");

		fprintf(fd_write_file, "%s\n", write_buf);
		close(fd);
		fclose(fd_write_file);
		usleep(1000000);
	};

    return 0;
}
