/*
 * From eeprom.c - Version for Kathrein UFS913
 * https://github.com/mozilla-b2g/i2c-tools/blob/master/tools/i2cbusses.c
 * 
 * Modify by Slash
 * slash.linux.c@gmail.com
 *
 * (c) 2011 konfetti
 * partly copied from uboot source!
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

/*
 * Description:
 *
 * Little utility to dump the EEPROM contents or show the
 * MAC address on Fortis receivers.
 * In effect this a stripped down version of ipbox_eeprom with minor
 * modifications and additions. Kudos to the original authors.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

#define CFG_EEPROM_ADDR  0x50
#define CFG_EEPROM_SIZE  256

#define EEPROM_WRITE
#if defined EEPROM_WRITE
#define CFG_EEPROM_PAGE_WRITE_DELAY_MS	11	/* 10ms. but give more */
#define CFG_EEPROM_PAGE_WRITE_BITS 4
#define	EEPROM_PAGE_SIZE  (1 << CFG_EEPROM_PAGE_WRITE_BITS)
#endif

int i2c_read(int fd_i2c, unsigned char addr, unsigned char reg, unsigned char *buffer, int len)
{
	struct i2c_rdwr_ioctl_data i2c_rdwr;
	int	err;
	unsigned char b0[] = {reg};

	//printf("%s > i2c addr = 0x%02x, reg = 0x%02x, len = %d\n", __func__, addr, reg, len);

	i2c_rdwr.nmsgs = 2;
	i2c_rdwr.msgs = malloc(2 * sizeof(struct i2c_msg));
	i2c_rdwr.msgs[0].addr = addr;
	i2c_rdwr.msgs[0].flags = 0;
	i2c_rdwr.msgs[0].len = 1;
	i2c_rdwr.msgs[0].buf = b0;

	i2c_rdwr.msgs[1].addr = addr;
	i2c_rdwr.msgs[1].flags = 1;
	i2c_rdwr.msgs[1].len = len;
	i2c_rdwr.msgs[1].buf = malloc(len);

	memset(i2c_rdwr.msgs[1].buf, 0, len);

	if ((err = ioctl(fd_i2c, I2C_RDWR, &i2c_rdwr)) < 0) {
		printf("[eeprom] %s i2c_read of reg 0x%02x failed %d %d\n", __func__, reg, err, errno);
		printf("         %s\n", strerror(errno));
		free(i2c_rdwr.msgs[0].buf);
		free(i2c_rdwr.msgs);
		return -1;
	}

	//printf("[eeprom] %s reg 0x%02x -> ret 0x%02x\n", __func__, reg, (i2c_rdwr.msgs[1].buf[0] & 0xff));
	memcpy(buffer, i2c_rdwr.msgs[1].buf, len);

	free(i2c_rdwr.msgs[1].buf);
	free(i2c_rdwr.msgs);

	//printf("[eeprom] %s <\n", __func__);
	return 0;
}

#if defined EEPROM_WRITE
int i2c_write(int fd_i2c, unsigned char addr, unsigned char reg, unsigned char *buffer, int len)
{
	struct 	i2c_rdwr_ioctl_data i2c_rdwr;
	int	err;
	unsigned char buf[256];

	//printf("%s > 0x%0x - %s - %d\n", __func__, reg, buffer, len);

	buf[0] = reg;
	memcpy(&buf[1], buffer, len);

	i2c_rdwr.nmsgs = 1;
	i2c_rdwr.msgs = malloc(1 * sizeof(struct i2c_msg));
	i2c_rdwr.msgs[0].addr = addr;
	i2c_rdwr.msgs[0].flags = 0;
	i2c_rdwr.msgs[0].len = len + 1;
	i2c_rdwr.msgs[0].buf = buf;

	if ((err = ioctl(fd_i2c, I2C_RDWR, &i2c_rdwr)) < 0)
	{
		//printf("i2c_read failed %d %d\n", err, errno);
		//printf("%s\n", strerror(errno));
		free(i2c_rdwr.msgs[0].buf);
		free(i2c_rdwr.msgs);
		return -1;
	}
	free(i2c_rdwr.msgs);
	//printf("%s <\n", __func__);
	return 0;
}
#endif

/* *************************** uboot copied func ************************************** */

#if defined EEPROM_WRITE
int eeprom_write(int fd, unsigned dev_addr, unsigned offset, unsigned char *buffer, unsigned cnt)
{
	unsigned end = offset + cnt;
	unsigned blk_off;
	int rcode = 0;

	/* Write data until done or would cross a write page boundary.
	 * We must write the address again when changing pages
	 * because the address counter only increments within a page.
	 */

	printf("[eeprom] %s >\n", __func__);
	while (offset < end)
	{
		unsigned len;
		unsigned char addr[2];

		blk_off = offset & 0xFF;    /* block offset */

		addr[0] = offset >> 8;      /* block number */
		addr[1] = blk_off;          /* block offset */
		addr[0] |= dev_addr;        /* insert device address */

		len = end - offset;

		if (i2c_write(fd, addr[0], offset, buffer, len) != 0)
		{
			rcode = 1;
		}
		buffer += len;
		offset += len;

		usleep(CFG_EEPROM_PAGE_WRITE_DELAY_MS * 1000);
	}
	printf("[eeprom] %s < (%d)\n", __func__, rcode);
	return rcode;
}
#endif

int eeprom_read(int fd, unsigned dev_addr, unsigned offset, unsigned char *buffer, unsigned cnt)
{
	unsigned end = offset + cnt;
	unsigned blk_off;
	int rcode = 0;
	int i;

	//printf("[eeprom] %s > offset 0x%03x, count %d bytes\n", __func__, offset, cnt);

	while (offset < end) {
		unsigned len;
		unsigned char addr[2];

		blk_off = offset & 0xFF;    /* block offset */

		addr[0] = offset >> 8;      /* block number */
		addr[1] = blk_off;          /* block offset */
		addr[0] |= dev_addr;        /* insert device address */

		len = cnt;
		//printf("[eeprom] %s address=0x%02x, reg=0x%02x\n", __func__, addr[0], addr[1]);

		for (i = 0; i < len; i++)
		{
			if (i2c_read(fd, addr[0], offset + i, &buffer[i], 1) != 0)
			{
				rcode = 1;
				break;
			}
		}
		buffer += len;
		offset += len;
	}
	//printf("[eeprom] %s < (%d)\n", __func__, rcode);
	return rcode;
}

int main(int argc, char *argv[])
{
	int fd_i2c, ret;
	int vLoop, i2c_addr;
	int i, rcode = 0;
	unsigned char buf[CFG_EEPROM_SIZE], write_date_sz;
	char dev_name[10];
	unsigned char buffer[10];
	unsigned char buf_write[10] = {
	0x00, 0xAA, 0xCC, 0xDD, 0x13,
	0x0a, 0x2A, 0x52, 0x69, 0xd2};

	printf("%s >\n", argv[1]);
	sprintf(dev_name, "%s", argv[1]);
	printf("dev_name:%s\n", dev_name);
	fd_i2c = open(dev_name, O_RDWR);
	if (fd_i2c <= 0) {
		printf("open %s fail\n", dev_name);
		return 0;
	}

	//ret = ioctl(fd_i2c, I2C_SLAVE, i2c_addr);
	ret = ioctl(fd_i2c, I2C_SLAVE_FORCE, CFG_EEPROM_ADDR);
	if (ret < 0) {
		printf("set I2C_SLAVE fail ret %d\n", ret);
		goto dev_fail;
	}

	write_date_sz = sizeof(buf_write) / sizeof(buf_write[0]);
	ret = eeprom_write(fd_i2c, CFG_EEPROM_ADDR, 0, buf_write, write_date_sz);
	if (ret < 0)
		goto dev_fail;

	rcode |= eeprom_read(fd_i2c, CFG_EEPROM_ADDR, 0, buf, sizeof(buf));
	if (rcode < 0) {
		rcode = 1;
		goto dev_fail;
	}
	printf("EEPROM dump\n");
	printf("Addr  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
	printf("-----------------------------------------------------\n");
	for (vLoop = 0; vLoop < CFG_EEPROM_SIZE; vLoop += 0x10) {
		printf(" %02x ", vLoop);
		for (i = 0; i < 0x10; i++) {
			printf(" %02x", buf[vLoop + i]);
		}
		printf("\n");
	}

dev_fail:
	close(fd_i2c);
	return 0;

failed:
	printf("[eeprom] failed <\n");
	return -1;
}
// vim:ts=4
