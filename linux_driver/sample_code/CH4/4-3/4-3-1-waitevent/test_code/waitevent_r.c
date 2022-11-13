#include <stdio.h>
#include <fcntl.h>

int main()
{
    int sleepytest;
    int code;

    sleepytest = open("/dev/waitevent",O_RDONLY );

    if ((code = read(sleepytest , NULL , 0)) < 0)
        printf("read error! code=%d \n",code);
    else   
        printf("read ok! code=%d \n",code);

    close(sleepytest);

    return 0;
}
