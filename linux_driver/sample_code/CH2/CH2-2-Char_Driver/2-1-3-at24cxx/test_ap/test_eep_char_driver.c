#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define DEVFILE "/dev/at24cxx-0"
#define DISABLE_GLIB_BUF

int main(void)
{
	unsigned char data[10] = {
		0x10, 0x13, 0x62, 0x32, 0x54,
		0x52, 0x36, 0x87, 0x28, 0x99
		};

	unsigned char data2[10] = {
		0xdd, 0xdd, 0xaa, 0xaa, 0x55,
		0x55, 0x11, 0x22, 0x33, 0x99
		};

	unsigned char read_data[10] = {0};

	int fd;
	int ret, i;

	fd = open(DEVFILE, O_RDWR);
	if (!fd) {
		perror("open");
	}

//	ret = write(fd, data, 10);
//	printf("fwrite : ret %d\r\n", ret);

	memset(read_data, 0xFF, sizeof(char) * 10);
	ret = read(fd, read_data, 10);
	for (i = 0; i < ret; i++)
		printf("fread : read_data 0x%x\r\n", read_data[i]);

//	ret = write(fd, data2, 10);
//	printf("fwrite : ret %d\r\n", ret);

//	memset(read_data, 0xFF, sizeof(char) * 10);
//	ret = read(fd, read_data, 10);
//	for (i = 0; i < ret; i++)
//		printf("fread : read_data 0x%x\r\n", read_data[i]);

	close(fd);
	return 0;
}
