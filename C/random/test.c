#include <stdio.h>
#include "random.h"

#define MAX(X,Y) ((X) > (Y) ?  (X) : (Y))
#define MIN(X,Y) ((X) < (Y) ?  (X) : (Y))

#define UPPER 100000000

int main()
{
    Ran r(0205670521);
    unsigned int even;
    int i,j;
    unsigned int a;
    for (j=1;j<=16;j++)
    {
        even=0;
        for (i=0;i< UPPER;i++)
        {
            a= r.int32();
            if (a & 2<<j)
               even++; 
        }
        unsigned int odd = i-even;
        double diff = (i/2 - MIN(even, (i-even))*1.0)/(i/2)*100;
        printf("bit %2d Even: %d, odd: %d, diff %g%%\n", j, even, i-even, diff);
    }
}
