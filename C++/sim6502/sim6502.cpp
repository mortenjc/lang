#include <cstdio>
#include <CPU.h>
#include <Memory.h>

std::vector<uint8_t> ldxyi_and_dec {
  0xA2, 0x22,       // LDX Imm 0x22
  0xA0, 0x44,       // LDY Imm 0x44
  0xE8,             // INX
  0xE8,             // INX
  0xC8,             // INY
  0xC8,             // INY
  0x88,             // DEY
  0x88,             // DEY
  0xCA,             // DEX
  0xCA,             // DEX
};

int main(int argc, char * argv[])
{
  Memory mem;
  CPU cpu(mem);

  mem.reset();
  mem.load(0x1000, ldxyi_and_dec);

  cpu.reset();
  int instructions = 0;
  while (instructions < 20) {
    uint8_t instruction = cpu.getInstruction();
    cpu.handleInstruction(instruction);
    instructions++;
  }

  return 0;
}
