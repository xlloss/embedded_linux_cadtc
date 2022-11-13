#include "test_inc/test.h"
#include <math.h>

int main(int argc, char **argv)
{

#ifndef D_FLAG
	printf("gnu_c_compiler\n");
#else
	printf("gnu_c_compiler -D flag TEST\n");
#endif

#ifdef I_FLAG
	print_test();
#endif

#ifdef L_FLAG
	printf("gnu_c_compiler -l flag TEST\n");
	printf("cos = %f\n", cos(60.0));
#endif

	return 0;
}
