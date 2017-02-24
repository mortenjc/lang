#include <stdio.h>
#include <stdlib.h>

unsigned int g2b(unsigned int num)
{
  num = num ^ (num >> 16);
  num = num ^ (num >> 8);
  num = num ^ (num >> 4);
  num = num ^ (num >> 2);
  num = num ^ (num >> 1);
  return num;
}

int main(int argc, char * argv[]) {

  for (int i = 0; i < 4095; i++) {
    printf("gray %d, decode %d\n", i, g2b(i));
  }

  return 0;
}
