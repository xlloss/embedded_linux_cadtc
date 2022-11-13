#include <stdio.h>

void func_add(int x, int y)
{
	printf("%s %d\n", __func__, __LINE__);
	printf("x + y = %d\n", x + y);
}

int main(int argc, char **argv)
{
	printf("%s %d\n", __func__, __LINE__);

	func_add(1, 5);

	return 0;
}
