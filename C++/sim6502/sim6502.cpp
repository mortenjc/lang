#include <cstdio>
#include <CPU.h>
#include <Instructions.h>
#include <Memory.h>

std::vector<uint8_t> ldxyi_and_dec {
  LDXI, 0x22,       // LDX Imm 0x22
  LDYI, 0x44,       // LDY Imm 0x44
  INX,             // INX
  INX,             // INX
  INY,             // INY
  INY,             // INY
  DEY,             // DEY
  DEY,             // DEY
  DEX,             // DEX
  DEX,             // DEX
  NOP
};

std::vector<uint8_t> countdown_y_from_10 {
  LDXI, 0x00,       // LDX Imm 0
  LDYI, 0x0A,       // LDY Imm 10
  DEY,              // DEY
  BNE, 0x82,        // BNE (-2, prev line)
  INX,              // INX
  INX,              // INX
  NOP
};

// Should be loaded @ 0x20
std::vector<uint8_t> data_zp {
  0xCD, 0xAB,   // 0xABCD
  0x76, 0x98    // 0x9876   sum = 0x4443 + carry
};

std::vector<uint8_t> add_16bit_at_x20 {
  CLC,
  LDAZP, 0x20,
  ADCZP, 0x22,
  STAZP, 0x24,
  LDAZP, 0x21,
  ADCZP, 0x23,
  STAZP, 0x25,
  NOP
};

int main(int argc, char * argv[])
{
  Memory mem;
  CPU cpu(mem);

  mem.reset();
  //mem.load(0x1000, ldxyi_and_dec);
  //mem.load(0x1000, countdown_y_from_10);
  mem.load(0x20, data_zp);
  mem.load(0x1000, add_16bit_at_x20);

  cpu.reset();

  bool running{true};

  while (running ) {
    uint8_t instruction = cpu.getInstruction();
    running = cpu.handleInstruction(instruction);
  }

  mem.dump(0x20, 6);

  return 0;
}
