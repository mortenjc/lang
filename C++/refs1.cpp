#include <cstdio>

static __inline__ unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

class Ref1 {
public:
  int& calc(int& a);
  int calc2(int a);

};

int& Ref1::calc(int& a) {
  a *=2;
  return a;
}

int Ref1::calc2(int a) {
  return a*2;
}


int main()
{
  Ref1 a;

  unsigned long long start = rdtsc();
  for (int i = 0; i < 10000000; i++) {
     int b = a.calc(i);
  }
  unsigned long long end = rdtsc();
  printf("Time spent: %llu\n", end - start);

  start = rdtsc();
  for (int i = 0; i < 10000000; i++) {
     int b = a.calc2(i);
  }
  end = rdtsc();
  printf("Time spent: %llu\n", end - start);

  return 0;
}
