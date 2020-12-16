

#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <vector>


class Memory {
public:
  uint8_t mem[65536];

  void clear() {
    memset(mem, 0, sizeof(mem));
  }

  // Clear memory and set program start address to 0x1000
  void reset() {
    clear();
    mem[0xfffc] = 0x00;
    mem[0xfffd] = 0x10;
  }

  void load(uint16_t address, std::vector<uint8_t> & program) {
    assert(address + program.size() < 65536);
    memcpy(mem + address, program.data(), program.size());
  }

  uint8_t readByte(uint16_t address) {
    return mem[address];
  }

  uint16_t readWord(uint16_t address) {
    assert(address < 0xFFFF);
    return mem[address] + mem[address + 1] * 256;
  }
};
