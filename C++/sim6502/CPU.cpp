

#include <cassert>
#include <CPU.h>
#include <Instructions.h>


void CPU::reset() {
  A = X = Y = 0;
  Status.mask = 0;
  PC = mem.readWord(0xFFFC);
  SP = 0x01ff;
}

void CPU::debug() {
  printf("0x%04x: A(%3d)  X(%3d)  Y(%3d) ",
    PC, A, X, Y);
  printFlags();
}

uint8_t CPU::getInstruction() {
   return mem.readByte(PC);
}

bool CPU::handleInstruction(uint8_t instruction) {
  int16_t pcinc{0};

  switch (instruction) {

    // CLEAR/SET flags
    case CLC: { // CLC - CLear Carry
      Status.bits.C = 0;
      pcinc = 1;
      debug();
      printf("CLC\n");
    }
    break;

    /// ADD SUBTRACT
    case ADCZP: { // Add with carry - zero page
      uint8_t zp = mem.readByte(PC + 1);
      uint8_t zpval = mem.readByte(zp);
      uint8_t tmp = A + zpval + Status.bits.C;
      //printf("\nA + zpval + C == 0x%02x + 0x%02x + %d\n", A, zpval, Status.bits.C);
      if ((A + zpval + Status.bits.C) > 255) {
        Status.bits.C = 1; // Set carry
      } else {
        Status.bits.C = 0; // Clear carry
      }
      A = tmp;
      updateStatus(A);
      pcinc = 2;
      debug();
      printf("ADC (ZP 0x%02x) %d\n", zp, A);
    }
    break;

    //
    // LOAD + STORE
    case LDAZP: { // LDA Zero Page
      uint8_t zp = mem.readByte(PC + 1);
      A = mem.readByte(zp);
      updateStatus(A);
      pcinc = 2;
      debug();
      printf("LDA (ZP 0x%02x) %d\n", zp, A);
    }
    break;

    case STAZP: { // STA Zero Page
      uint8_t zp = mem.readByte(PC + 1);
      mem.writeByte(zp, A);
      pcinc = 2;
      debug();
      printf("STA (ZP 0x%02x) %d\n", zp, A);
    }
    break;

    case LDXI: { // LDX Imm
      X = mem.readByte(PC + 1);
      updateStatus(X);
      pcinc = 2;
      debug();
      printf("LDX (I) %d\n", X);
    }
    break;

    case LDYI: { // LDY Imm
      Y = mem.readByte(PC + 1);
      updateStatus(Y);
      pcinc = 2;
      debug();
      printf("LDY (I) %d\n", Y);
    }
    break;

    // INC and DEC
    case INX: { // INX
      X++;
      updateStatus(X);
      pcinc = 1;
      debug();
      printf("INX\n");
    }
    break;

    case INY: { // INY
      Y++;
      updateStatus(Y);
      pcinc = 1;
      debug();
      printf("INY\n");
    }
    break;

    case DEX: { // DEX
      X--;
      updateStatus(X);
      pcinc = 1;
      debug();
      printf("DEX\n");
    }
    break;

    case DEY: { // DEY
      Y--;
      updateStatus(Y);
      pcinc = 1;
      debug();
      printf("DEY\n");
    }
    break;

    // BRANCHES
    case BNE: { // BNE - Branch if nor Zero
      uint8_t val = mem.readByte(PC + 1);
      if (Status.bits.Z != 1) {
          if (val & 0x80) {
            pcinc = - (val - 0x80) + 1;
          } else {
            pcinc = val;
          }
      } else {
        pcinc = 2;
      }
      debug();
      printf("BNE 0x%02x (%d)\n", val, pcinc);
    }
    break;

    /// MISC
    case NOP: { // No operation
      pcinc = 1;
      debug();
      printf("NOP\n");
    }
    break;

    default:
      debug();
      printf("Illegal instruction: 0x%02x\nexiting...\n", instruction);
      return false;
  }
  PC += pcinc;
  return true;
}
