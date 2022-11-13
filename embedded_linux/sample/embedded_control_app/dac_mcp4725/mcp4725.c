/***************************************************************************
    copyright            : (C) by 2003-2004 Stefano Barbato
    email                : stefano@codesink.org

    $Id: 24cXX.c,v 1.5 2004/02/29 11:05:28 tat Exp $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/fs.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include "mcp4725.h"


static inline __s32 i2c_smbus_access(int file, char read_write, __u8 command, 
                                     int size, union i2c_smbus_data *data)
{
	struct i2c_smbus_ioctl_data args;

	args.read_write = read_write;
	args.command = command;
	args.size = size;
	args.data = data;

	return ioctl(file, I2C_SMBUS, &args);
}

static inline __s32 i2c_smbus_read_byte(int file)
{
	union i2c_smbus_data data;

	if (i2c_smbus_access(file, I2C_SMBUS_READ, 0, I2C_SMBUS_BYTE, &data))
		return -1;
	else
		return 0xFF & data.byte;
}

static inline __s32 i2c_smbus_write_byte(int file, __u8 value)
{
	return i2c_smbus_access(file, I2C_SMBUS_WRITE, value,
	                        I2C_SMBUS_BYTE, NULL);
}

static inline __s32 i2c_smbus_write_byte_data(int file, __u8 command, 
                                              __u8 value)
{
	union i2c_smbus_data data;
	data.byte = value;
	return i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
	                        I2C_SMBUS_BYTE_DATA, &data);
}

static inline __s32 i2c_smbus_read_word_data(int file, __u8 command)
{
	union i2c_smbus_data data;
	if (i2c_smbus_access(file, I2C_SMBUS_READ, command,
	                     I2C_SMBUS_WORD_DATA, &data))
		return -1;
	else
		return 0xFFFF & data.word;
}

static inline __s32 i2c_smbus_write_word_data(int file, __u8 command, 
                                              __u16 value)
{
	union i2c_smbus_data data;
	data.word = value;
	return i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
	                        I2C_SMBUS_WORD_DATA, &data);
}

static inline __s32 i2c_smbus_write_i2c_block_data(int file, __u8 command,
                                               __u8 length, __u8 *values)
{
	union i2c_smbus_data data;
	int i;

	if (length > 32)
		length = 32;

	for (i = 1; i <= length; i++)
		data.block[i] = values[i-1];

	data.block[0] = length;

	return i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
	                        I2C_SMBUS_I2C_BLOCK_DATA, &data);
}

/* Returns the number of read bytes */
static inline __s32 i2c_smbus_block_process_call(int file, __u8 command,
                                                 __u8 length, __u8 *values)
{
	union i2c_smbus_data data;
	int i;

	if (length > 32)
		length = 32;

	for (i = 1; i <= length; i++)
		data.block[i] = values[i-1];

	data.block[0] = length;

	if (i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
            I2C_SMBUS_BLOCK_PROC_CALL, &data)) {
		return -1;
    } else {
		for (i = 1; i <= data.block[0]; i++)
			values[i-1] = data.block[i];
		return data.block[0];
	}
}

static int i2c_write_1b(struct dac *d_dev, __u8 buf)
{
	int r;
	/* 
     * we must simulate a plain I2C byte write with SMBus functions
     */
	r = i2c_smbus_write_byte(d_dev->fd, buf);
	if(r < 0)
		fprintf(stderr, "Error i2c_write_1b: %s\n", strerror(errno));

	usleep(10);
	return r;
}

static int i2c_write_2b(struct dac *d_dev, __u8 buf[2])
{
	int r;
	/* 
     * we must simulate a plain I2C byte write with SMBus functions
     */
	r = i2c_smbus_write_byte_data(d_dev->fd, buf[0], buf[1]);
	if(r < 0)
		fprintf(stderr, "Error i2c_write_2b: %s\n", strerror(errno));

	usleep(10000);
	return r;
}

static int i2c_write_3b(struct dac *d_dev, __u8 buf[3])
{
	int r;

	/* 
     * we must simulate a plain I2C byte write with SMBus functions
	 * the __u16 data field will be byte swapped by the SMBus protocol
     */
	r = i2c_smbus_write_word_data(d_dev->fd, buf[0], buf[2] << 8 | buf[1]);
	if(r < 0)
		fprintf(stderr, "Error i2c_write_3b: %s\n", strerror(errno));

	usleep(10);
	return r;
}


#define CHECK_I2C_FUNC(var, label) \
	do { if (0 == (var & label)) { \
		fprintf(stderr, "\nError: " \
			#label " function is required. Program halted.\n\n"); \
		exit(1); } \
	} while(0);

int dac_open(char *dev_fqn, int addr, int type, struct dac *d_dev)
{
	unsigned long funcs, fd, r;

	fd = open(dev_fqn, O_RDWR);
	if (fd <= 0) {
		fprintf(stderr, "Error dac_open: %s\n", strerror(errno));
		return -1;
	}

	if ((r = ioctl(fd, I2C_FUNCS, &funcs) < 0)) {
		fprintf(stderr, "Error dac_open: %s\n", strerror(errno));
		return -1;
	}

	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_READ_BYTE );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_WRITE_BYTE );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_READ_BYTE_DATA );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_WRITE_BYTE_DATA );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_READ_WORD_DATA );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_WRITE_WORD_DATA );

	if((r = ioctl(fd, I2C_SLAVE, addr)) < 0) {
		printf ("%s %d\n", __func__, __LINE__);
		fprintf(stderr, "Error dac_open: %s\n", strerror(errno));
		return -1;
	}

	d_dev->fd = fd;
	d_dev->addr = addr;
	d_dev->dev = dev_fqn;
	d_dev->type = type;

	return 0;
}

int dac_close(struct dac *d_dev)
{
	close(d_dev->fd);
	d_dev->fd = -1;
	d_dev->dev = 0;
	d_dev->type = DAC_TYPE_UNKNOWN;

	return 0;
}

int dac_write_package(struct dac *d_dev, unsigned int data)
{
    __u8 w2b_buf[2];
    __u8 w3b_buf[3];

    if (d_dev->type != DAC_TYPE_FAST_MODE &&
        d_dev->type != DAC_TYPE_NORMAL_MODE) {
		fprintf(stderr, "ERR: unknown DAC command type\n");
		return -1;
    }

	if(d_dev->type == DAC_TYPE_FAST_MODE) {
		printf("type DAC_TYPE_FAST_MODE\n");
        w2b_buf[1] = data;
        w2b_buf[0] = (data >> 8) & 0xff;
		return i2c_write_2b(d_dev, w2b_buf);
	} else {
        //w3b_buf[0] = mem_addr;
        //w3b_buf[1] = (data >> 8) & 0xff;
        //w3b_buf[2] = data & 0xff;
        //usleep(10000);

		return i2c_write_3b(d_dev, w3b_buf);
	}
}
