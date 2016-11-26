#include <stdio.h>

#define TRC_LVL_ONE 1
#define TRC_LVL_TWO 2
#define TRC_LVL_THREE 3

#define TRACE_LEVEL TRC_LVL_THREE
//#define TRACE_LEVEL TRC_LVL_ONE

#define TRACE(l, ...) \
   (void)(( TRC_LVL_ ##l >= TRACE_LEVEL) ? \
     printf(__VA_ARGS__) : \
    0)


int main() {

  asm("nop");
  TRACE(ONE, "Trace ONE made it\n");
  asm("nop");
  TRACE(TWO, "Trace TWO made it\n");
  asm("nop");
  TRACE(THREE, "Trace THREE made it\n");
  asm("nop");
  return 0;
}
