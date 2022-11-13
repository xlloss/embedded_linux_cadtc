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
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "mcp4725.h"


#define IIC_DEV "/dev/i2c-2"
#define WRITE_MASK 0x01
#define DAC_SLAVE_ADDR (0xC0 >> 1)
#define HEX_FMT 16

#define die_if(a, msg) do { do_die_if( a , msg, __LINE__); } while(0);
void do_die_if(int b, char* msg, int line)
{
	if (!b)
		return;
	fprintf(stderr, "Error at line %d: %s\n", line, msg);
	fprintf(stderr, "	sysmsg: %s\n", strerror(errno));
	exit(1);
}

static int write_to_dac(struct dac *d_dev, unsigned int data)
{
	fflush(stdout);
	die_if(dac_write_package(d_dev, data), "write error");
	return 0;
}

int main(int argc, char** argv)
{
	struct dac dac_dev;
    int option_index = 0;
    int opts;
    unsigned char cmd;
    unsigned int data;

    struct option long_options[] = {
        {"write", no_argument, 0, 'w'},
        {0, 0, 0, 0}
    };

    while (1) {
        opts = getopt_long (argc, argv, "rw:",
        long_options, &option_index);

        /* Detect the end of the options. */
        if (opts == -1)
            break;

        switch (opts) {
        case 'w':
            cmd = cmd | WRITE_MASK;
            data = strtol(optarg, NULL, HEX_FMT);
            printf("data 0x%x\n", data);
            break;

        case '?':
            printf("i2c -w 0x0fff\n");
            printf("i2c -r\n");
            break;
        default:
                break;
        }
    }

    if (!cmd) {
        printf("i2c -w 0x0fff\n");
        return 0;
    }

    if (dac_open(IIC_DEV, DAC_SLAVE_ADDR, DAC_TYPE_FAST_MODE, &dac_dev) < 0) {
        printf ("dac_open fail\n");
        return 0;
    }


    switch (cmd) {

    case WRITE_MASK:
        write_to_dac(&dac_dev, data & 0xFFF);
        break;

    default:
		printf("Writing value range is 0x0000 - 0x0fff\n");
        printf("./mcp4725 -w 0x0fff\n");
        break;
    }

    dac_close(&dac_dev);

	return 0;
}

