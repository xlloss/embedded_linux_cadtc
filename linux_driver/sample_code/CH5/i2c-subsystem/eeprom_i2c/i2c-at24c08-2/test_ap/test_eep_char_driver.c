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

	unsigned char data2[10] = {
		0xdd, 0xa3, 0x52, 0x24, 0xd2,
		0xe2, 0xd6, 0xc7, 0xe2, 0x54
    };
	unsigned char data_cle[10] = {0};
	unsigned char read_data[10] = {0};

	int fd;
	int ret, i;
	off_t offset;

	fd = open(DEVFILE, O_RDWR);
	if (!fd) {
		perror("open");
	}

	ret = write(fd, data_cle, 10);

	offset = 0;
	lseek(fd, offset, SEEK_SET);
	ret = write(fd, data, 10);
	printf("write : ret %d\r\n", ret);

	offset = 0;
	lseek(fd, offset, SEEK_SET);

	offset = 20;
	lseek(fd, offset, SEEK_SET);
	ret = write(fd, data2, 10);
	printf("write : ret 0x%x\r\n", ret);

	offset = 20;
	lseek(fd, offset, SEEK_SET);
	ret = read(fd, read_data, 10);
	printf("read : ret 0x%x\r\n", ret);

	for (i = 0; i < 10; i++) {
		printf("fread : read_data curr 0x%x\r\n", read_data[i]);
	}

	offset = 9;
	lseek(fd, offset, SEEK_SET);
	printf("seek to %d byte\r\n", (unsigned int)offset);

	ret = read(fd, read_data, 1);
	printf("fread : read_data curr 0x%x\r\n", read_data[0]);

	offset = 10;
	data[0] = 0x77;
	printf("seek to %d byte\r\n", (unsigned int)offset);

	ret = write(fd, data, 1);
	printf("fwrite : curr 0x%x\r\n", data[0]);

	ret = read(fd, read_data, 1);
	printf("fread : read_data curr 0x%x\r\n", read_data[0]);


	offset = 9;
	lseek(fd, offset, SEEK_SET);
	printf("seek to %d byte\r\n", (unsigned int)offset);

	ret = read(fd, read_data, 1);
	printf("fread : read_data curr 0x%x\r\n", read_data[0]);

	close(fd);
	return 0;
}
