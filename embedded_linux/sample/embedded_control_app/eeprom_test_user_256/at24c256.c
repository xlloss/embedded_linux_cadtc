#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <linux/fs.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

#define IIC_DEV "/dev/i2c-2"

#define CHECK_I2C_FUNC(var, label) \
	do { if (0 == (var & label)) { \
		fprintf(stderr, "\nError: " \
			#label " function is required. Program halted.\n\n"); \
		exit(1); } \
	} while(0);

#define EEPROM_SLAVE_ADDR 0x50
#define WRITE_MASK 0x01
#define READ_MASK 0x02

int i2c_smbus_ctrl(int file, char read_write, unsigned char command,
 union i2c_smbus_data *data, int size)
{
	struct i2c_smbus_ioctl_data args;

	args.read_write = read_write;
	args.command = command;
	args.size = size;
	args.data = data;

	return ioctl(file, I2C_SMBUS, &args);
}

int main(int argc, char** argv)
{
	unsigned long funcs;
    int option_index = 0;
    int opts;
	int fd, ret;
	unsigned int index = 0;
	unsigned char data = 0, cmd = 0;
	unsigned char epp_hi_addr, epp_lo_addr;
	unsigned char epp_data;
	unsigned char command;
	union i2c_smbus_data smbus_data_wr;
	union i2c_smbus_data smbus_data_rd;

    struct option long_options[] = {
        {"write", no_argument, 0, 'w'},
        {"read",  no_argument, 0, 'r'},
        {"index",  required_argument, 0, 'i'},
        {"data",  required_argument, 0, 'd'},
        {"?",  required_argument, 0, '?'},
        {0, 0, 0, 0}
    };

    while (1) {
        opts = getopt_long (argc, argv, "wri:d:", long_options, &option_index);

        /* Detect the end of the options. */
        if (opts == -1)
            break;

        switch (opts) {
        case 'w':
            cmd = WRITE_MASK;
            break;

        case 'r':
            cmd = READ_MASK;
            break;

        case 'i':
            index = strtol(optarg, NULL, 16);
            break;

        case 'd':
            data = strtol(optarg, NULL, 16);
            break;

        case '?':
            printf(
                "./eeprom_i2c_test -r -i 10\r\n"
                "./eeprom_i2c_test -w -i 10 -d 0x20\r\n");
            break;
        default:
                break;
        }
    }

	fd = open(IIC_DEV, O_RDWR);
	if (fd <= 0) {
		fprintf(stderr, "Error eeprom_open: %s\n", strerror(errno));
		return -1;
	}

	/* get funcs list */
	ret = ioctl(fd, I2C_FUNCS, &funcs);
	if (ret < 0) {
		fprintf(stderr, "Error eeprom_open: %s\n", strerror(errno));
		return -1;
	}

	/* check for req funcs */
	CHECK_I2C_FUNC(funcs, I2C_FUNC_SMBUS_READ_BYTE);
	CHECK_I2C_FUNC(funcs, I2C_FUNC_SMBUS_WRITE_BYTE);
	CHECK_I2C_FUNC(funcs, I2C_FUNC_SMBUS_READ_BYTE_DATA);
	CHECK_I2C_FUNC(funcs, I2C_FUNC_SMBUS_WRITE_BYTE_DATA);
	CHECK_I2C_FUNC(funcs, I2C_FUNC_SMBUS_READ_WORD_DATA);
	CHECK_I2C_FUNC(funcs, I2C_FUNC_SMBUS_WRITE_WORD_DATA);

	/* set working device */
	ret = ioctl (fd, I2C_SLAVE, EEPROM_SLAVE_ADDR);
	if (ret < 0) {
		fprintf(stderr, "Error eeprom_open: %s\n", strerror(errno));
		return -1;
	}

	if (cmd == WRITE_MASK) {
		/* set write data to at24c256 */
		epp_hi_addr = index >> 8;
		epp_lo_addr = index & 0xFF;
		epp_data = data;
		command = epp_hi_addr;

		smbus_data_wr.word = ((epp_data << 8) | epp_lo_addr);
		ret = i2c_smbus_ctrl(fd, I2C_SMBUS_WRITE, command, &smbus_data_wr, I2C_SMBUS_WORD_DATA);
		if (ret < 0) {
			printf("I2C_SMBUS_WRITE fail\n");
			return 0;
		}
		usleep(5000);
	} else if (cmd == READ_MASK) {
		/* set read address of at24c256 */
		epp_hi_addr = index >> 8;
		epp_lo_addr = index & 0xFF;
		command = epp_hi_addr;

		smbus_data_wr.byte = epp_lo_addr;
		i2c_smbus_ctrl(fd, I2C_SMBUS_WRITE, command, &smbus_data_wr, I2C_SMBUS_BYTE_DATA);
		if (ret < 0) {
			printf("I2C_SMBUS_WRITE fail\n");
			return 0;
		}

		usleep(5000);
		/* read data of at24c256 */
		command = 0;
		ret = i2c_smbus_ctrl(fd, I2C_SMBUS_READ, command, &smbus_data_rd, I2C_SMBUS_BYTE);
		if (ret < 0) {
			printf("I2C_SMBUS_READ fail\n");
			return 0;
		}
		printf("read = 0x%x\n", smbus_data_rd.byte);
	}

	return 0;
}
