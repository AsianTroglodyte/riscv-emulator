#include "unity/src/unity.h"
#include "rv64i.h"
#include <inttypes.h>

void setUp(void) {}

void tearDown(void) {}

void test_LW(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_WORDS; i++) {
    memory[i * 4] = i * 4;
  }

  // POSITIVE NUMBERS
  uint32_t instruction = create_i_type(4, 4, LW, 10, LOAD);
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(4, registers[10]);

  // NEGATIVE IMMEDIATE
  registers[4] = 8;
  uint32_t instruction1 = create_i_type(-4, 4, LW, 10, LOAD);
  run_instruction(instruction1, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(4, registers[10]);

  // NEGATIVE NUMBERS
  registers[4] = 0;
  memory[4] = (int8_t)-4;
  memory[5] = 255;
  memory[6] = 255;
  memory[7] = 255;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(UINT64_MAX-3, registers[10]);
}

void test_LB(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};

  for (uint32_t i = 0; i < MEMORY_BYTES; i++) {
    memory[i] = i;
  }

  uint32_t instruction = create_i_type(10, 4, LB, 10, LOAD);
  // POSITIVE NUMBERS
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(10, registers[10]);

  // NEGATIVE NUMBERS
  memory[10] = (int8_t)-4;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(UINT64_MAX-3, registers[10]);

  // NEGATIVE IMMEDIATE
  registers[4] = 8;
  uint32_t instruction1 = create_i_type(-4, 4, LB, 10, LOAD);
  run_instruction(instruction1, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(4, registers[10]);
}

void test_LH(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_HALF_WORDS; i++) {
    memory[i * 2] = i * 2;
  }

  uint32_t instruction = create_i_type(10, 4, LH, 10, LOAD);
  // POSITIVE NUMBERS
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(10, registers[10]);

  // NEGATIVE NUMBERS
  memory[10] = (int8_t)-4;
  memory[11] = 255;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(UINT64_MAX-3, registers[10]);

  // NEGATIVE IMMEDIATE
  registers[4] = 8;
  uint32_t instruction1 = create_i_type(-4, 4, LH, 10, LOAD);
  run_instruction(instruction1, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(4, registers[10]);
}

void test_LBU(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_BYTES; i++) {
    memory[i] = i ;
  }

  // POSITIVE NUMBERS IN MEMORY
  uint32_t instruction = create_i_type(10, 4, LBU, 10, LOAD);
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(10, registers[10]);

  // NEGATIVE NUMBERS IN MEMORY
  memory[10] = (int8_t)-4;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(UINT8_MAX - 3, registers[10]);

  // NEGATIVE IMMEDIATE
  registers[4] = 8;
  uint32_t instruction1 = create_i_type(-4, 4, LBU, 10, LOAD);
  run_instruction(instruction1, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(4, registers[10]);
}

void test_LHU(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_HALF_WORDS; i++) {
    memory[i * 2] = i * 2;
  }

  // POSITIVE NUMBERS
  uint32_t instruction = create_i_type(10, 4, LHU, 10, LOAD);
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(10, registers[10]);

  // NEGATIVE NUMBERS
  memory[10] = (int8_t)-4;
  memory[11] = 255;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(UINT16_MAX - 3, registers[10]);

  // NEGATIVE IMMEDIATE
  registers[4] = 8;
  memory[4] = 4;
  uint32_t instruction1 = create_i_type(-4, 4, LHU, 10, LOAD);
  run_instruction(instruction1, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(4, registers[10]);
}

void test_SB(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_BYTES; i++) {
    memory[i] = i;
  }

  // TEST WITH ZERO
  registers[10] = 0;
  uint32_t instruction = create_s_type(10, 10, 4, SB, STORE);
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(0, memory[10]);

  // TEST WITH STORING NEGATIVE VALUE
  registers[10] = -10;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(UINT8_MAX - 9, get_byte(memory, 10));

  /* // TEST WITH POSITIVE */
  registers[10] = 5;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(5, get_byte(memory, 10));

  /* // TEST WITH NEGATIVE IMMEDIATE */
  registers[4] = 20;
  registers[10] = 5;
  uint32_t instruction_2 = create_s_type(-10, 10, 4, SB, STORE);
  run_instruction(instruction_2, memory, registers);
  TEST_ASSERT_EQUAL(5, get_byte(memory, 10));
}

void test_SH(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_HALF_WORDS; i++) {
    memory[i * 2] = i * 2;
  }
  // TEST WITH ZERO
  registers[10] = 0;
  uint32_t instruction = create_s_type(10, 10, 4, SH, STORE);
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(0, get_half_word(memory, 10));

  // TEST WITH STORING NEGATIVE VALUE
  registers[10] = -10;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(UINT16_MAX - 9, get_half_word(memory, 10));

  /* // TEST WITH POSITIVE */
  registers[10] = 5;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(5, get_half_word(memory, 10));

  /* // TEST WITH NEGATIVE IMMEDIATE */
  registers[4] = 20;
  registers[10] = 5;
  uint32_t instruction_2 = create_s_type(-10, 10, 4, SH, STORE);
  run_instruction(instruction_2, memory, registers);
  TEST_ASSERT_EQUAL(5, get_half_word(memory, 10));
}

void test_SW(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_WORDS; i++) {
    memory[i * 4] = i * 4;
  }

  // TEST WITH ZERO
  registers[8] = 0;
  uint32_t instruction = create_s_type(8, 10, 4, SW, STORE);
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(0, get_word(memory, 8));

  // TEST WITH STORING NEGATIVE VALUE
  registers[10] = -10;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(UINT32_MAX - 9, get_word(memory, 8));

  /* // TEST WITH POSITIVE */
  registers[10] = 5;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(5, get_word(memory, 8));

  /* // TEST WITH NEGATIVE IMMEDIATE */
  registers[4] = 16;
  registers[10] = 5;
  uint32_t instruction_2 = create_s_type(-8, 10, 4, SW, STORE);
  run_instruction(instruction_2, memory, registers);
  TEST_ASSERT_EQUAL(5, get_word(memory, 8));
}

void test_SD(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_WORDS / 2; i++) {
    memory[i * 8] = i * 8;
  }

  // TEST WITH ZERO
  uint32_t instruction = create_s_type(8, 10, 4, SD, STORE);
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(0, get_double_word(memory, 8));

  // TEST WITH STORING NEGATIVE VALUE
  registers[10] = -10;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(UINT64_MAX - 9, get_double_word(memory, 8));

  /* /\* // TEST WITH POSITIVE *\/ */
  registers[10] = 5;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(5, get_double_word(memory, 8));

  /* /\* // TEST WITH NEGATIVE IMMEDIATE *\/ */
  registers[4] = 16;
  registers[10] = 5;
  uint32_t instruction_2 = create_s_type(-8, 10, 4, SD, STORE);
  run_instruction(instruction_2, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(5, get_double_word(memory, 8));
}


void test_s_create(void) {
  // EASY: 1000 is within the signed 12-bit S-immediate range.
  uint32_t s_instruction_1 = create_s_type(1000, 6, 7, SB, STORE);
  // imm[11:5]=0011111, rs2=00110, rs1=00111, funct3=000, imm[4:0]=01000
  TEST_ASSERT_EQUAL(0b00111110011000111000010000100011, s_instruction_1);

  // NEGATIVE IMMEDIATE
  uint32_t s_instruction_2 = create_s_type(-1, 6, 7, SB, STORE);
  // should be imme_11_5(1111 111)(0 0110 rs2) (rs1 0011 1)(000 funct3) (imm_0_4 1111 1)(010 0011 opcode)
  TEST_ASSERT_EQUAL(0b11111110011000111000111110100011, s_instruction_2);

  // NEGATIVE IMMEDIATE BOUNDARY
  uint32_t s_instruction_3 = create_s_type(-2048, 6, 7, SB, STORE);
  // should be imme_11_5(1000 000)(0 0110 rs2) (rs1 0011 1)(000 funct3) (imm_0_4 0000 0)(010 0011 opcode)
  TEST_ASSERT_EQUAL(0b10000000011000111000000000100011, s_instruction_3);

  // POSITIVE IMMEDIATE BOUNDARY
  uint32_t s_instruction_4 = create_s_type(2047, 6, 7, SB, STORE);
  // should be imme_11_5(0111 111)(0 0110 rs2) (rs1 0011 1)(000 funct3) (imm_0_4 1111 1)(010 0011 opcode)
  TEST_ASSERT_EQUAL(0b01111110011000111000111110100011, s_instruction_4);
}


void test_get_s_immediate(void) {
  // EASY
  uint32_t s_instruction_1 = create_s_type(1000, 6, 7, SB, STORE);
  TEST_ASSERT_EQUAL(1000, get_s_immediate(s_instruction_1));

  // NEGATIVE IMMEDIATE
  uint32_t s_instruction_2 = create_s_type(-1, 6, 7, SB, STORE);
  TEST_ASSERT_EQUAL(-1, get_s_immediate(s_instruction_2));

  // NEGATIVE IMMEDIATE BOUNDARY
  uint32_t s_instruction_3 = create_s_type(-2048, 6, 7, SB, STORE);
  // should be imme_11_5(1000 000)(0 0110 rs2) (rs1 0011 1)(000 funct3) (imm_0_4 0000 0)(010 0011 opcode)
  TEST_ASSERT_EQUAL(-2048, get_s_immediate(s_instruction_3));

  // POSITIVE IMMEDIATE BOUNDARY
  uint32_t s_instruction_4 = create_s_type(2047, 6, 7, SB, STORE);
  // should be imme_11_5(0111 111)(0 0110 rs2) (rs1 0011 1)(000 funct3) (imm_0_4 1111 1)(010 0011 opcode)
  TEST_ASSERT_EQUAL(2047, get_s_immediate(s_instruction_4));
}


int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_LW);
  RUN_TEST(test_LH);
  RUN_TEST(test_LB);
  RUN_TEST(test_LBU);
  RUN_TEST(test_LHU);
  RUN_TEST(test_SB);
  RUN_TEST(test_SH);
  RUN_TEST(test_SW);
  RUN_TEST(test_SD);
  RUN_TEST(test_s_create);
  RUN_TEST(test_get_s_immediate);

  UNITY_END();
}
