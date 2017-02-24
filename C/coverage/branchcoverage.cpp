#include<stdio.h>

int func(int a, int b)
{
   int ret = 0;
   if (a > 5 && b < 7) {
      printf("First branch\n");
      ret = 1;
   } else {
      printf("Second branch\n");
      ret = 2;
   }
   return ret; 
}

int main() {
  func(1,1); 
}
