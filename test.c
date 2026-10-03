#include "unity/src/unity.h"
#include "rv64i.h"
#include <inttypes.h>
#define MEMORY_SIZE 32

void setUp(void) {}

void tearDown(void) {

}

void test_LW(void) {
  const uint32_t immediate_val = 10;
  const uint32_t rs_num = 10;
  const uint32_t rd_num = 10;

  uint64_t registers[NUM_REGISTERS] = {};
  uint32_t memory[MEMORY_SIZE] = {};
  for (uint32_t i = 0; i < MEMORY_SIZE; i++) {
    memory[i] = i;

}
  uint32_t instruction = {
    create_i_type(immediate_val, rs_num, LW, rd_num, LOAD)
  };

  run_instruction(instruction, memory, registers);


  TEST_ASSERT_EQUAL(10, registers[10]);
}

void test_LB(void) {
  const uint32_t immediate_val = 10;
  const uint32_t rs_num = 10;
  const uint32_t rd_num = 10;

  uint64_t registers[NUM_REGISTERS] = {};
  uint32_t memory[MEMORY_SIZE] = {};
  for (uint32_t i = 0; i < MEMORY_SIZE; i++) {
    memory[i] = i;

}
  uint32_t instruction = {
    create_i_type(immediate_val, rs_num, LW, rd_num, LOAD)
  };

  run_instruction(instruction, memory, registers);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_LW);

  RUN_TEST(test_LW_fail);

  UNITY_END();
}
