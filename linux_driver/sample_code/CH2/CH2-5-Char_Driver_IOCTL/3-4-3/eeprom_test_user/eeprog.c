/***************************************************************************
    copyright            : (C) by 2003-2004 Stefano Barbato
    email                : stefano@codesink.org
    website		 : http://codesink.org/eeprog.html

    $Id: eeprog.c,v 1.28 2004/02/29 11:06:41 tat Exp $
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
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "24cXX.h"


#define IIC7_DEV "/dev/i2c-7"

//0000 0RWA
#define ALL_MASK 0x01
#define WRITE_MASK 0x02
#define READ_MASK 0x04
#define EEPROM_SLAVE_ADDR 0x50


#define usage_if(a) do { do_usage_if( a , __LINE__); } while(0);
void do_usage_if(int b, int line)
{
	const static char *eeprog_usage =
		"I2C-24C08(256 bytes) Read/Write Program, ONLY FOR TEST!\n"
		"Base on 'eeprog' by Stefano Barbato (http://codesink.org/eeprog.html)\n"
		"i.e : i2c -r, i2c-w\r\n"
		"FriendlyARM Computer Tech. 2009\n"
		"Modify by Slash. 2016\n"
		"Modify by Slash. 2018\n";
	if(!b)
		return;
	fprintf(stderr, "%s\n[line %d]\n", eeprog_usage, line);
	exit(1);
}


#define die_if(a, msg) do { do_die_if( a , msg, __LINE__); } while(0);
void do_die_if(int b, char* msg, int line)
{
	if(!b)
		return;
	fprintf(stderr, "Error at line %d: %s\n", line, msg);
	fprintf(stderr, "	sysmsg: %s\n", strerror(errno));
	exit(1);
}

static int read_from_eeprom_index(struct eeprom *e, int addr)
{
	int ch;

	printf("addr = 0x%x\r\n", addr);

	die_if((ch = eeprom_read_byte(e, addr)) < 0, "read error");

	printf("read : index = 0x%x, data = 0x%x", addr, ch);

	fflush(stdout);
	fprintf(stderr, "\n\n");
	return 0;
}


static int read_from_eeprom(struct eeprom *e, int addr, int size)
{
	int ch, i;
	for(i = 0; i < size; ++i, ++addr)
	{
		die_if((ch = eeprom_read_byte(e, addr)) < 0, "read error");
		if( (i % 16) == 0 )
			printf("\n %.4x|  ", addr);
		else if( (i % 8) == 0 )
			printf("  ");
		printf("%.2x ", ch);
		fflush(stdout);
	}
	fprintf(stderr, "\n\n");
	return 0;
}

static int write_to_eeprom_index(struct eeprom *e, int addr, unsigned char data)
{
    die_if(eeprom_write_byte(e, addr, data), "write error");
    printf("write : index = 0x%x, data = 0x%x", addr, data);
	fflush(stdout);
	fprintf(stderr, "\n\n");
	return 0;
}

static int write_to_eeprom(struct eeprom *e, int addr)
{
	int i;

	for(i = 0, addr = 0; i < 256; i++, addr++) {
		if( (i % 16) == 0 )
			printf("\n %.4x|  ", addr);
		else if ((i % 8) == 0)
			printf("  ");

		printf("%.2x ", i);
		fflush(stdout);
		die_if(eeprom_write_byte(e, addr, i), "write error");
	}

	fprintf(stderr, "\n\n");
	return 0;
}


int main(int argc, char** argv)
{
	struct eeprom e;
    int option_index = 0;
    int opts;
    unsigned char index = 0, data = 0, cmd = 0;

    /* getopt_long stores the option index here. */
    struct option long_options[] = {
        {"all", no_argument, 0, 'a'},
        {"write", no_argument, 0, 'w'},
        {"read",  no_argument, 0, 'r'},
        {"index",  required_argument, 0, 'i'},
        {"data",  required_argument, 0, 'd'},
        {"?",  required_argument, 0, '?'},
        {0, 0, 0, 0}
    };

    while (1) {
        opts = getopt_long (argc, argv, "wrai:d:",
        long_options, &option_index);

        /* Detect the end of the options. */
        if (opts == -1)
            break;

        switch (opts) {
        case 'w':
            cmd = cmd | WRITE_MASK;
            break;

        case 'r':
            cmd = cmd | READ_MASK;
            break;

        case 'i':
            index = strtol(optarg, NULL, 16);
            break;

        case 'd':
            data = strtol(optarg, NULL, 16);
            break;

        case 'a':
            cmd = cmd | ALL_MASK;
            break;

        case '?':
            printf(
                "./i2c -w -a\r\n"
                "./i2c -r -a\r\n"
                "./i2c -r -i 0\r\n"
                "./i2c -r -i 10\r\n"
                "./i2c -w -a\r\n"
                "./i2c -w -i 10 -d 20\r\n"
                "./i2c -w -i 0 -d 4\r\n");
            break;
        default:
                break;
        }
    }

    if (!cmd) {
        printf(
            "./i2c -w -a\r\n"
            "./i2c -r -a\r\n"
            "./i2c -r -i 0\r\n"
            "./i2c -r -i 10\r\n"
            "./i2c -w -a\r\n"
            "./i2c -w -i 10 -d 20\r\n"
            "./i2c -w -i 0 -d 4\r\n");
        return 0;
    }

    if (eeprom_open(IIC7_DEV,
            EEPROM_SLAVE_ADDR, EEPROM_TYPE_8BIT_ADDR, &e) < 0) {
        printf ("eeprom_open fail\n");
        return 0;
    }
	read_from_eeprom_index(&e, 0);
	goto exit;

    switch (cmd) {

    case (ALL_MASK | WRITE_MASK):
        fprintf(stderr, "Writing 0x00-0xff into 24C08\n");
        write_to_eeprom(&e, 0);
        break;

    case (ALL_MASK | READ_MASK):
        fprintf(stderr, "Reading 256 bytes from 0x0\n");
        read_from_eeprom(&e, 0, 256);
        break;

    case READ_MASK:
        read_from_eeprom_index(&e, index);
        break;

    case WRITE_MASK:
        write_to_eeprom_index(&e, index, data);
        break;

    default:
        printf("???\r\n");
        break;
    }

exit:
    eeprom_close(&e);

	return 0;
}
