#include <stdio.h>
#include <helloworld.h>

void main(void)
{
	test = 1;
	printf("Hello World ! test %d -- %s %d\r\n", test, __func__, __LINE__);
}
