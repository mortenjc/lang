#include <iostream>
#include <stdio.h>
#include <stdlib.h>

class RandomAccess {
public:

  RandomAccess() {
    printf("Constructor\n");
  }

  void generateOffsets(int iterations) {
    if (iterations > MAX_ITERATIONS) {
      throw std::runtime_error("Number of iterations is too high");
    }
    for (int i = 0; i < iterations; i++) {
      printf("i: %d\n", i);
      Offsets[i] = 0;
    }
    Iterations = iterations;
  }

  void randomMemoryAccess() {
    for (int i = 0; i < Iterations; i++) {
      uint32_t Value = *((uint32_t *)(Memory + Offsets[i]));
    }
  }

private:
  static const int MAX_ITERATIONS = 200000000;
  static const int MAX_MEMORY = 10000000;
  uint16_t Offsets[MAX_ITERATIONS];
  uint8_t Memory[MAX_MEMORY];
  int Iterations{0};
};


int main(int argc, char * argv[]) {
  if (argc != 3) {
    printf("Usage: a.out iterations max_jump\n");
    exit(0);
  }

  int Iterations = atoi(argv[1]);
  int MaxJump = atoi(argv[2]);
  printf("Iterations %d, Maximum distance %d\n", Iterations, MaxJump);

  RandomAccess ra;
  //ra.generateOffsets(Iterations);
  //ra.randomMemoryAccess();
}
