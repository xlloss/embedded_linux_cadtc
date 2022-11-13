#include <stdlib.h>
#include <stdio.h>
#include "./hello_world_inc/hello_world.h"

int func1(void)
{
	printf("%s %d\n", __func__, __LINE__);

	return 0;
}
