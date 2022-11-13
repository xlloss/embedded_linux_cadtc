/*
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>


#define DEVFILE "/dev/char-led-0"

#define LED_0 (0 << 2)
#define LED_1 (1 << 2)
#define LED_2 (2 << 2)
#define LED_3 (3 << 2)
#define LED_ON  0
#define LED_OFF 1

int main(int argc, char *argv[])
{
    int fd;
    int id = 0;
    unsigned char led_cmd = 0;
    unsigned char led_state = 0;

	if (argv[1] == NULL || argv[2] == NULL) {
		printf("For Example : \r\n");
		printf("            : ledtest led_0 on\r\n");
		printf("            : ledtest led_0 off\r\n");
		return 0;
	}


	if (!strcmp(argv[1], "led_0")) {
		printf("set led_0\r\n");
		led_cmd = LED_0;
	} else if (!strcmp(argv[1], "led_1")) {
		printf("set led_1\r\n");
		led_cmd = LED_1;
	} else if (!strcmp(argv[1], "led_2")) {
		printf("set led_2\r\n");
		led_cmd = LED_2;
	} else if (!strcmp(argv[1], "led_3")) {
		printf("set led_3\r\n");
		led_cmd = LED_3;
	} else {
		printf("For Example : \r\n");
		printf("            : ledtest led_0 on\r\n");
		printf("            : ledtest led_0 off\r\n");
		return 0;
	}

    fd = open(DEVFILE, O_RDWR);
    if (fd == -1) {
        perror("open");
    }

    if (!strcmp(argv[2], "on")) {
        led_cmd = led_cmd & ~LED_ON;
    } else if (!strcmp(argv[2], "off")) {
        led_cmd = led_cmd | LED_OFF;
    } else {
        led_cmd = led_cmd & ~LED_ON;
    }

    write(fd, &led_cmd, 1);

    read(fd, &led_state, 1);
    printf("led_state = 0x%x\n", led_state);
    
    if (close(fd) != 0) {
        perror("close");
    }
}
