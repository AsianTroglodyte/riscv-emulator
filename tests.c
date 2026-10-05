#include "unity/src/unity.h"
#include "rv64i.h"
#include <inttypes.h>

void setUp(void) {}

void tearDown(void) {

}

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

  // NEGATIVE NUMBERS
  memory[4] = (int8_t)-4;
  memory[5] = 255;
  memory[6] = 255;
  memory[7] = 255;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(-4, registers[10]);
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
  TEST_ASSERT_EQUAL(10, registers[10]);

  // NEGATIVE NUMBERS
  memory[10] = (int8_t)-4;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(-4, registers[10]);
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
  TEST_ASSERT_EQUAL(10, registers[10]);

  // NEGATIVE NUMBERS
  memory[10] = (int8_t)-4;
  memory[11] = 255;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(-4, registers[10]);
}

void test_LBU(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_BYTES; i++) {
    memory[i] = i ;
  }

  // POSITIVE NUMBERS
  uint32_t instruction = create_i_type(10, 4, LBU, 10, LOAD);
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(10, registers[10]);

  // NEGATIVE NUMBERS
  memory[10] = (int8_t)-4;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(252, registers[10]);
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
  TEST_ASSERT_EQUAL(10, registers[10]);

  // NEGATIVE NUMBERS
  memory[10] = (int8_t)-4;
  memory[11] = 255;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(65532, registers[10]);
}

void test_SB(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_BYTES; i++) {
    memory[i] = i;
  }

  // TEST WITH ZERO
  uint32_t instruction = create_i_type(10, 4, SB, 10, STORE);
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(0, memory[10]);

  // TEST WITH NEGATIVE
  registers[10] = -10;
  /* print_registers(registers); */
  run_instruction(instruction, memory, registers);
  print_memory(memory);
  TEST_ASSERT_EQUAL(246, get_byte(memory, 10));

  // TEST WITH POSITIVE
  registers[10] = 10;
  run_instruction(instruction, memory, registers);
  print_memory(memory);
  TEST_ASSERT_EQUAL(10, get_byte(memory, 10));
}

void test_SH(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_HALF_WORDS; i++) {
    memory[i * 2] = i * 2;
  }

  // TEST WITH ZERO
  uint32_t instruction = create_i_type(10, 4, SH, 10, STORE);
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(0, get_half_word(memory , 10));

  // TEST WITH NEGATIVE
  registers[10] = -10;
  /* print_registers(registers); */
  run_instruction(instruction, memory, registers);
  print_memory(memory);
  TEST_ASSERT_EQUAL(65526, get_half_word(memory, 10));

  // TEST WITH POSITIVE
  registers[10] = 10;
  run_instruction(instruction, memory, registers);
  print_memory(memory);
  TEST_ASSERT_EQUAL(10, get_half_word(memory, 10));
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

  UNITY_END();
}
