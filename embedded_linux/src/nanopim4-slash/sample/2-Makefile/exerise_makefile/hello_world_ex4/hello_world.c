#include <stdlib.h>
#include <stdio.h>
#include "./hello_world_inc/hello_world.h"

int main(int argc, char **argv)
{
	int i;
	printf("Hello World\n");

	i = func1();
	printf("i = %d\n", i);

	return 0;
}
