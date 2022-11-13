/* 
 * Copy from Terence
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>

#define DEVICE_NAME "/dev/sema_test0"

char input = 0;

void *write_fun(void *ptr)
{
    int w_fd;
    char w_buf = 'a';

    w_fd = open(DEVICE_NAME, O_RDWR);
    if (w_fd == -1) {
        printf("Error in opening w_fd \n");
        return;
    }
    while (input != 'q') {
        printf ("== Write %c ==\r\n", w_buf);
        write(w_fd, &w_buf, sizeof(w_buf));

        if (w_buf == 'z')
            w_buf = 'a';
        else
            w_buf = w_buf + 1;
    };
    close(w_fd);
    //printf("%s Exit\r\n", __func__);
}

void *read_fun(void *ptr)
{
    int r_fd;
    char r_buf = 0;

    printf("%s\r\n", __func__);

    r_fd = open(DEVICE_NAME, O_RDWR);
    if (r_fd == -1) {
        printf("Error in opening r_fd \n");
        return;
    }

    while (input != 'q') {
        read(r_fd, &r_buf, sizeof(r_buf));
        printf ("== User Level Read character is %c ==\n", r_buf);
    };
    close(r_fd);
    //printf("%s Exit\r\n", __func__);
}

int main (int argc, char *argv[])
{
    pthread_t th_read, th_write;

    int  iret1, iret2;
    int ret;

    if (argc < 2) {
        return 0;
    }

    printf("argv[0] %s\r\n", argv[0]);
    printf("argv[1] %s\r\n", argv[1]);

    if (!strcmp("read", argv[1])) {
        iret1 = pthread_create(&th_read, NULL, read_fun, NULL);
        if (iret1) {
            fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
            return -1;
        }
    } else if (!strcmp("write", argv[1])) {

        iret2 = pthread_create(&th_write, NULL, write_fun, NULL);
        if (iret2) {
            fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
            return -1;
        }
    } else {
        printf("userage\r\n");
        return 0;
    }

    while (1) {
        input = getchar();
        if (input == 'q') {
            break;
        }
        sleep(1);
    };

    pthread_join(th_write, NULL);
    pthread_join(th_read, NULL);

    return 0;
}

