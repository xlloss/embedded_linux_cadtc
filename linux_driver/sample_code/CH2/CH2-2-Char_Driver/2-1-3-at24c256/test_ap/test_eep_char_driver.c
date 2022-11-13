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
		0x11, 0x22, 0x33, 0x44, 0x55,
		0x66, 0x77, 0x88, 0x99, 0xaa
	};

	unsigned char read_data[10] = {0};

	int fd;
	int ret, i;

	fd = open(DEVFILE, O_RDWR);
	if (!fd) {
		perror("open");
	}

	ret = write(fd, data, 10);
	printf("fwrite : ret %d\r\n", ret);

	if (lseek(fd, 0, SEEK_SET) < 0)
		printf("lseek fail\n");

	sleep(1);
	ret = read(fd, read_data, 10);
	for (i = 0; i < ret; i++)
		printf("fread : read_data 0x%x\r\n", read_data[i]);

	close(fd);
	return 0;
}
