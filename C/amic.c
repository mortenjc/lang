#include <stdio.h>
#include <math.h>

void amic(int c, int n, int div)
{
   int goal = c;
   int i, res = 0;
   for (i = 0 ; i < n; i++) {
      res += pow(c/div, n);
      c -= (c/div)*div;
      div /= 10;
      if (res > goal)
         return;
   }
   //printf("goal: %d, res: %d\n", goal, res);
   if (res == goal)
     printf("%d is cool\n", res);
}

int main(int argc, char * argv[])
{
   int n, c, lo, hi;

   for (n=2;n < 13; n++) {
     printf("Numbers with %d digits\n", n);
     lo = pow(10,n-1);
     hi = pow(10,n) - 1;
     for (c = lo; c <= hi; c++) {
        amic(c, n, lo);
     }
   }
}
