#include <iostream>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

class RandomAccess {
public:
  RandomAccess() {
    Offsets.reserve(MAX_ITERATIONS);
    Memory.reserve(MAX_MEMORY + 16);
    MemoryOffset = &Memory[0];
    printf("Constructor\n");
  }

  void generateOffsets(int iterations, int maxjump) {
    std::uniform_int_distribution<int> distribution(0, maxjump);
    if (iterations > MAX_ITERATIONS) {
      throw std::runtime_error("Number of iterations is too high");
    }
    for (int i = 0; i < iterations; i++) {
      int address = distribution(generator);
      printf("i: %d, a: %d\n", i, address);
      Offsets[i] = address;
    }
    Iterations = iterations;
  }

  void randomMemoryAccess() {
    for (int i = 0; i < Iterations; i++) {
      uint32_t Value = *((uint32_t *)(MemoryOffset + Offsets[i]));
    }
  }

private:
  std::default_random_engine generator;

  static const int MAX_ITERATIONS = 200000000;
  static const int MAX_MEMORY = 10000000;
  std::vector<uint16_t> Offsets;
  std::vector<uint8_t> Memory;
  uint8_t * MemoryOffset;
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
  ra.generateOffsets(Iterations, MaxJump);

  ra.randomMemoryAccess();
}
