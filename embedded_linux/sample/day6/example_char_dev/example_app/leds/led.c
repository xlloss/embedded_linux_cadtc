#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char **argv)
{
	int fd, ret;
	char buf;
	int arg = 0, opt_select = 0;

	if (argc < 2) {
		printf("plz input number");
		printf("--please select options ----------------------------------\n");
		printf("--1 test IOCTL LED Light----------------------------------\n");
		printf("--2 test IOCTL LED Dark-----------------------------------\n");
		printf("--3 test Read  (read count have to 1) LED GPIO 0-4 status-\n");
		printf("--4 test Write (write count have to 1) LED Light----------\n");
		printf("----------------------------------------------------------\n");
		return 0;
	}

	sscanf(argv[1], "%d", &opt_select);
	printf("opt_select %d\n", opt_select);

	fd = open("/dev/tiny-4412-0", O_RDWR);
	if (fd < 0) {
		perror("open device leds\n");
		exit(1);
	}
	
	switch (opt_select) {
	case 1:
		/* led light */
		ioctl(fd, 0x12345, arg);
		break;
	case 2:
	/* led dark */
		ioctl(fd, 0x54321, arg);
		break;
	case 3:
		read(fd, &buf, 1);
		break;
	case 4:
		buf = 0x0f;
		ret = write(fd, &buf, 1);
		break;
	default:
	return 0;
	};

	close(fd);

	return 0;
}

