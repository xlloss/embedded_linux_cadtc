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
		0x11, 0x11, 0x22, 0x22, 0x33,
		0x33, 0x44, 0x44, 0x55, 0x00
		};

	unsigned char data2[10] = {
		0xdd, 0xdd, 0xaa, 0xaa, 0xbb,
		0xbb, 0xcc, 0xcc, 0xff, 0xff
		};

	unsigned char data3[10] = {
		0xee, 0xee, 0xee, 0xee, 0xee,
		0xaa, 0xaa, 0xaa, 0xaa, 0xaa
		};

	unsigned char data_clr[255];
	unsigned char read_data[10] = {0};

	int fd;
	int ret, i;
	off_t offset;

	fd = open(DEVFILE, O_RDWR);
	if (!fd) {
		perror("open");
	}

	memset(data_clr, 0xff, 255);
	offset = 0;
	lseek(fd, offset, SEEK_SET);
	ret = write(fd, &data_clr, 255);

	offset = 0;
	lseek(fd, offset, SEEK_SET);
	ret = write(fd, data, 10);
	printf("write : ret 0x%x\r\n", ret);

	offset = 0;
	lseek(fd, offset, SEEK_SET);
	ret = read(fd, read_data, 10);
	printf("read : ret 0x%x\r\n", ret);

	for (i = 0; i < 10; i++)
		printf("fread : read_data curr 0x%x\r\n", read_data[i]);

	offset = 20;
	lseek(fd, offset, SEEK_SET);
	ret = write(fd, data2, 10);
	printf("write date2 : ret 0x%x\r\n", ret);

	offset = 20;
	lseek(fd, offset, SEEK_SET);
	ret = read(fd, read_data, 10);
	printf("read : ret 0x%x\r\n", ret);

	for (i = 0; i < 10; i++)
		printf("fread : read_data2 curr 0x%x\r\n", read_data[i]);

	offset = 100;
	lseek(fd, offset, SEEK_SET);
	printf("seek to %d byte\r\n", (unsigned int)offset);

	ret = write(fd, data3, 10);
	printf("write data3: ret 0x%x\r\n", ret);

	offset = 100;
	lseek(fd, offset, SEEK_SET);
	printf("seek to %d byte\r\n", (unsigned int)offset);
	ret = read(fd, read_data, 10);

	for (i = 0; i < 10; i++)
		printf("fread : read_data3 curr 0x%x\r\n", read_data[i]);

	close(fd);
	return 0;
}
