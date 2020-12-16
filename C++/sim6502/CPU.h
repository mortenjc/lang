

#pragma once

#include <Memory.h>
#include <cstdint>
#include <string>


class CPU {
public:
  uint8_t A, X, Y;
  uint16_t PC;
  uint16_t SP;
  union {
    struct {
      uint8_t C : 1;
      uint8_t Z : 1;
      uint8_t I : 1;
      uint8_t D : 1;
      uint8_t B : 1;
      uint8_t O : 1;
      uint8_t N : 1;
    } bits;
    uint8_t mask;
  } Status;
  Memory & mem;

  CPU(Memory & memory) : mem(memory) {};

  // Reset CPU - clear registers, set program counter
  void reset();


  void printFlags() {
    printf(" [%c%c%c%c%c%c%c] ",
        Status.bits.C ? 'c' : ' ' ,
        Status.bits.Z ? 'z' : ' ' ,
        Status.bits.I ? 'i' : ' ' ,
        Status.bits.D ? 'd' : ' ' ,
        Status.bits.B ? 'b' : ' ' ,
        Status.bits.O ? 'o' : ' ' ,
        Status.bits.N ? 'n' : ' ' );
  }

  uint8_t getInstruction();

  // This is where the action is
  bool handleInstruction(uint8_t instruction);

  // Updates Zero and Negative flags based on value
  void updateStatus(uint8_t value) {
    Status.bits.Z = 0;
    Status.bits.N = 0;
    if (value == 0) {
      Status.bits.Z = 1;
    }
    if (value & 0x80) {
      Status.bits.N = 1;
    }
  }

  void updepri(uint8_t & val, std::string text) {
    updateStatus(val);
    debug();
    printf("%s\n", text.c_str());
  }

  // print out registers and flags
  void debug();

};
