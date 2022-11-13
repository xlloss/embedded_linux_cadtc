#include <stdio.h>
#include <fcntl.h>

int main()
{

    int sleepytest;
    int code;
    
    sleepytest = open("/dev/waitevent",O_WRONLY );

    if ((code = write(sleepytest , NULL , 0)) < 0 )
        printf("write error\n");
    else   
        printf("write ok! code=%d \n",code);

    close(sleepytest);
    return 0;
}

