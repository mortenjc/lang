

#include <cassert>
#include <CPU.h>


void CPU::reset() {
  A = X = Y = 0;
  Status.mask = 0;
  PC = mem.readWord(0xFFFC);
  SP = 0x01ff;
}

void CPU::debug() {
  printf("0x%04x - 0x%04x: A(%3d)  X(%3d)  Y(%3d), flags: 0x%02x   ",
    PC, SP, A, X, Y, Status.mask);
}

uint8_t CPU::getInstruction() {
   return mem.readByte(PC);
}

void CPU::handleInstruction(uint8_t instruction) {
  uint16_t pcinc{0};

  assert(instruction != 0);

  switch (instruction) {

    //
    // LOADS
    case 0xA2: { // LDX Imm
      X = mem.readByte(PC + 1);
      updateStatus(X);
      pcinc = 2;
      debug();
      printf("LDX (I) %d\n", X);
    }
    break;

    case 0xA0: { // LDY Imm
      Y = mem.readByte(PC + 1);
      updateStatus(Y);
      pcinc = 2;
      debug();
      printf("LDY (I) %d\n", Y);
    }
    break;

    // INC and DEC
    case 0xE8: { // INX
      X++;
      updateStatus(X);
      pcinc = 1;
      debug();
      printf("INX\n");
    }
    break;

    case 0xC8: { // INY
      Y++;
      updateStatus(Y);
      pcinc = 1;
      debug();
      printf("INY\n");
    }
    break;

    case 0xCA: { // DEX
      X--;
      updateStatus(X);
      pcinc = 1;
      debug();
      printf("DEX\n");
    }
    break;

    case 0x88: { // DEY
      Y--;
      updateStatus(Y);
      pcinc = 1;
      debug();
      printf("DEY\n");
    }
    break;

    default:
      printf("Illegal instruction: 0x%02x\n", instruction);
  }
  PC += pcinc;
}
