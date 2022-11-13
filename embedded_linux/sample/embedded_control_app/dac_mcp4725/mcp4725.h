/***************************************************************************
    copyright            : (C) by 2003-2004 Stefano Barbato
    email                : stefano@codesink.org

    $Id: 24cXX.h,v 1.6 2004/02/29 11:05:28 tat Exp $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef _mpc4725XX_H_
#define _mpc4725XX_H_
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

#define DAC_TYPE_UNKNOWN 0
#define DAC_TYPE_FAST_MODE	0x1
#define DAC_TYPE_NORMAL_MODE 0x2

struct dac
{
	char *dev; 	// device file i.e. /dev/i2c-N
	int addr;	// i2c address
	int fd;		// file descriptor
	int type; 	// dac type
};

/*
 * opens the dac device at [dev_fqn] (i.e. /dev/i2c-N) whose address is
 * [addr] and set the dac device
 */
int dac_open(char *dev_fqn, int addr, int type, struct dac*);

/*
 * closees the dac device [e] 
 */
int dac_close(struct dac *d_dev);

/*
 * writes [data] at memory address [mem_addr] 
 * Note: dac must have been selected by ioctl(fd,I2C_SLAVE, address) 
 */
int dac_write_package(struct dac *d_dev, unsigned int data);

#endif
