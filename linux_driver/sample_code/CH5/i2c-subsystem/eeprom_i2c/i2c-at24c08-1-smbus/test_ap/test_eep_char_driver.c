#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define DEVFILE "/dev/at24cxx-0"
#define DISABLE_GLIB_BUF

int main(void)
{
	unsigned char data[10] = {
		0x10, 0x13, 0x62, 0x32, 0x54,
		0x52, 0x36, 0x87, 0x28, 0x99
	};

	unsigned char read_data[10] = {0};

	int fd;
	int ret, i;
	off_t offset;

	fd = open(DEVFILE, O_RDWR);
	if (!fd) {
		perror("open");
	}

	ret = write(fd, data, 10);
	printf("fwrite : ret %d\r\n", ret);

	offset = 5;
	lseek(fd, offset, SEEK_SET);
	ret = read(fd, read_data, 1);
	for (i = 0; i < ret; i++)
		printf("fread : read_data 0x%x\r\n", read_data[i]);

	close(fd);
	return 0;
}
