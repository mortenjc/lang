#include <stdio.h>

int main(int argc, char * argv[])
{
   int i, t;

   printf("Marias fede 20*20 tabel\n"); 
   for (i = 1; i <= 20; i++) {
      for (t = 1; t <= 20; t++) {
         if (t%2 == 0)
            printf("%3d ", i*t);
         else
            printf("   ");
      }
      printf("\n");
   }

   return 0;
}
