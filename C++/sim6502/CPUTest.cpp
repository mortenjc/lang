
#include <gtest/gtest.h>
#include <Memory.h>
#include <CPU.h>
#include <Instructions.h>

class CPUTest: public ::testing::Test {
protected:
   Memory mem;
   CPU * cpu;

   // Helper for load instructions
   void load(uint8_t opcode, uint8_t value, uint8_t & reg) {
     mem.writeByte(0x1000, opcode);
     mem.writeByte(0x1001, value);
     cpu->PC = 0x1000;
     uint8_t inst = cpu->getInstruction();
     ASSERT_EQ(inst, opcode);
     cpu->handleInstruction(inst);
     ASSERT_EQ(value, reg);
   }

   void SetUp( ) {
     mem.reset();
     for (int i = 0; i < 256; i++) {
       mem.writeByte(i, i);
     }
     cpu = new CPU(mem);
     cpu->reset();
   }

   void TearDown( ) { }
};


TEST_F(CPUTest, INX) {
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




TEST_F(CPUTest, LOADI) {
  load(LDXI, 0x42, cpu->X);
  ASSERT_EQ(cpu->Status.bits.Z, 0);
  ASSERT_EQ(cpu->Status.bits.N, 0);

  load(LDXI, 0x00, cpu->X);
  ASSERT_EQ(cpu->Status.bits.Z, 1);
  ASSERT_EQ(cpu->Status.bits.N, 0);

  load(LDXI, 0x80, cpu->X);
  ASSERT_EQ(cpu->Status.bits.Z, 0);
  ASSERT_EQ(cpu->Status.bits.N, 1);

  load(LDYI, 0x42, cpu->Y);
  ASSERT_EQ(cpu->Status.bits.Z, 0);
  ASSERT_EQ(cpu->Status.bits.N, 0);

  load(LDYI, 0x00, cpu->Y);
  ASSERT_EQ(cpu->Status.bits.Z, 1);
  ASSERT_EQ(cpu->Status.bits.N, 0);

  load(LDYI, 0x80, cpu->Y);
  ASSERT_EQ(cpu->Status.bits.Z, 0);
  ASSERT_EQ(cpu->Status.bits.N, 1);
}

TEST_F(CPUTest, LOADZP) {
  load(LDAZP, 0x42, cpu->A);
  ASSERT_EQ(cpu->Status.bits.Z, 0);
  ASSERT_EQ(cpu->Status.bits.N, 0);

  load(LDAZP, 0x00, cpu->A);
  ASSERT_EQ(cpu->Status.bits.Z, 1);
  ASSERT_EQ(cpu->Status.bits.N, 0);

  load(LDAZP, 0x80, cpu->A);
  ASSERT_EQ(cpu->Status.bits.Z, 0);
  ASSERT_EQ(cpu->Status.bits.N, 1);

  load(LDXZP, 0x42, cpu->X);
  ASSERT_EQ(cpu->Status.bits.Z, 0);
  ASSERT_EQ(cpu->Status.bits.N, 0);

  load(LDXZP, 0x00, cpu->X);
  ASSERT_EQ(cpu->Status.bits.Z, 1);
  ASSERT_EQ(cpu->Status.bits.N, 0);

  load(LDXZP, 0x80, cpu->X);
  ASSERT_EQ(cpu->Status.bits.Z, 0);
  ASSERT_EQ(cpu->Status.bits.N, 1);
}

TEST_F(CPUTest, ANDI) {
  mem.writeByte(0x1000, ANDI);
  mem.writeByte(0x1001, 0xFF);
  cpu->PC = 0x1000;
  cpu->A = 0x7F;
  uint8_t inst = cpu->getInstruction();
  ASSERT_EQ(inst, ANDI);
  cpu->handleInstruction(inst);
  ASSERT_EQ(cpu->A, 0x7F);
  ASSERT_EQ(cpu->Status.bits.Z, 0);
  ASSERT_EQ(cpu->Status.bits.N, 0);

  mem.writeByte(0x1000, ANDI);
  mem.writeByte(0x1001, 0xFF);
  cpu->PC = 0x1000;
  cpu->A = 0x80;
  inst = cpu->getInstruction();
  ASSERT_EQ(inst, ANDI);
  cpu->handleInstruction(inst);
  ASSERT_EQ(cpu->A, 0x80);
  ASSERT_EQ(cpu->Status.bits.Z, 0);
  ASSERT_EQ(cpu->Status.bits.N, 1);
}

TEST_F(CPUTest, JSR) {
  ASSERT_EQ(cpu->SP, 0x1FF);
  mem.writeByte(0x1000, JSR);
  mem.writeByte(0x1001, 0x00);
  mem.writeByte(0x1002, 0x20);
  mem.writeByte(0x1003, NOP);
  mem.writeByte(0x2000, RTS);
  cpu->PC = 0x1000;
  uint8_t inst = cpu->getInstruction();
  ASSERT_EQ(inst, JSR);
  cpu->handleInstruction(inst);
  ASSERT_EQ(cpu->SP, 0x1FD);
  ASSERT_EQ(cpu->PC, 0x2000);

  inst = cpu->getInstruction();
  ASSERT_EQ(inst, RTS);
  cpu->handleInstruction(inst);
  ASSERT_EQ(cpu->SP, 0x1FF);
  ASSERT_EQ(cpu->PC, 0x1003);

  inst = cpu->getInstruction();
  ASSERT_EQ(inst, NOP);
  cpu->handleInstruction(inst);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
