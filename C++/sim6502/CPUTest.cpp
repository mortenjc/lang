
#include <gtest/gtest.h>
#include <Memory.h>
#include <CPU.h>
#include <Instructions.h>

class CPUTest: public ::testing::Test {
protected:
   Memory mem;
   CPU * cpu;

   void SetUp( ) {
     mem.reset();
     cpu = new CPU(mem);
     cpu->reset();
   }

   void TearDown( ) { }
};

TEST_F(CPUTest, INX) {
  mem.reset();
  mem.writeByte(0x1000, INX);
  for (uint16_t i = 1; i < 256; i++) {
    cpu->PC = 0x1000;
    uint8_t inst = cpu->getInstruction();
    ASSERT_EQ(inst, INX);
    cpu->handleInstruction(inst);
    ASSERT_EQ(i, cpu->X);
    ASSERT_EQ(cpu->Status.bits.Z, 0);
    if (cpu->X & 0x80) {
      ASSERT_EQ(cpu->Status.bits.N, 1);
    } else {
      ASSERT_EQ(cpu->Status.bits.N, 0);
    }
  }

  // Handle wrap to zero
  cpu->PC = 0x1000;
  uint8_t inst = cpu->getInstruction();
  ASSERT_EQ(inst, INX);
  cpu->handleInstruction(inst);
  ASSERT_EQ(0, cpu->X);
  ASSERT_EQ(cpu->Status.bits.Z, 1);
}

TEST_F(CPUTest, INY) {
  mem.reset();
  mem.writeByte(0x1000, INY);
  for (uint16_t i = 1; i < 256; i++) {
    cpu->PC = 0x1000;
    uint8_t inst = cpu->getInstruction();
    ASSERT_EQ(inst, INY);
    cpu->handleInstruction(inst);
    ASSERT_EQ(i, cpu->Y);
    ASSERT_EQ(cpu->Status.bits.Z, 0);
    if (cpu->Y & 0x80) {
      ASSERT_EQ(cpu->Status.bits.N, 1);
    } else {
      ASSERT_EQ(cpu->Status.bits.N, 0);
    }
  }

  // Handle wrap to zero
  cpu->PC = 0x1000;
  uint8_t inst = cpu->getInstruction();
  ASSERT_EQ(inst, INY);
  cpu->handleInstruction(inst);
  ASSERT_EQ(0, cpu->Y);
  ASSERT_EQ(cpu->Status.bits.Z, 1);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
