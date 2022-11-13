#include <stdio.h>
#include <sample_prog.h>

int main()
{
   int x;
   testme = 1;
   ctest1(&x);
   printf("Valx=%d\n", x);
   printf("testme=%d\n", testme);

   return 0;
}
