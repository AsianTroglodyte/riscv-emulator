#include "unity/src/unity.h"
#include "rv64i.h"
#include <inttypes.h>

void setUp(void) {}

void tearDown(void) {

}

void test_LW(void) {
  const uint32_t immediate_val = 10;
  const uint32_t rs_num = 10;
  const uint32_t rd_num = 10;

  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_WORDS; i++) {
    memory[i * 4] = i * 4;
  }

  uint32_t instruction = create_i_type(4, 4, LW, 10, LOAD);


  /* print_registers(registers); */
  run_instruction(instruction, memory, registers);
  /* print_registers(registers); */
  print_memory(memory);
  TEST_ASSERT_EQUAL(4, registers[10]);
}

/* void test_LB(void) { */
/*   const uint32_t immediate_val = 10; */
/*   const uint32_t rs_num = 10; */
/*   const uint32_t rd_num = 10; */

/*   uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5}; */
/*   uint8_t memory[MEMORY_WORDS] = {}; */
/*   for (uint32_t i = 0; i < MEMORY_WORDS; i++) { */
/*     memory[i] = i; */
/*   } */

/*   uint32_t instruction = { */
/*     create_i_type(10, 10, LW, 10, LOAD), */
/*   }; */

/*   run_instruction(instruction, memory, registers); */
/*   TEST_ASSERT_EQUAL(10, registers[10]); */
/* } */

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_LW);

  /* RUN_TEST(test_LW_fail); */

  UNITY_END();
}
