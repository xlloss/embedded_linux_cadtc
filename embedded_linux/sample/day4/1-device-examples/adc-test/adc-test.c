#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <errno.h>
#include <string.h>

#define ADC_SET_CHANNEL         0xc000fa01
#define ADC_SET_ADCTSC          0xc000fa02

#define CHANNELCOUNT 1
int main(int argc, char* argv[])
{
    int channels[CHANNELCOUNT] = {0};
    int channel;
    int i = 0;
    char output[255];
    char buffer[30];
    int fd = open("/dev/tiny4412-adc", 0);
	int len;
	int value = -1;
	char outbuff[255];

	fprintf(stderr, "press Ctrl-C to stop\n");
	if (fd < 0) {
		perror("open ADC device:");
		return -1;
	}

	for(;;) {
        output[0] = 0;
        for (i = 0; i < CHANNELCOUNT; i++) {
            channel = channels[i];
            if (ioctl(fd, ADC_SET_CHANNEL, channel) < 0) {
                perror("Can't set channel for /dev/adc!");
                close(fd);
                return -1;
            }
            
            len = read(fd, buffer, sizeof buffer -1);
            if (len > 0) {
                buffer[len] = '\0';
                sscanf(buffer, "%d", &value);
                sprintf(outbuff, "AIN%d %d\n", channel, value);
                strcat(output, outbuff);
            } else {
                perror("read ADC device:");
                close(fd);
                return -1;
            }
        }
        printf("%s", output);
        printf("==============\n");
    }
	close(fd);
}
