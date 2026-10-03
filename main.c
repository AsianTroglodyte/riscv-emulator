#include "rv64i.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#define MEMORY_SIZE 32

uint32_t create_i_type(uint32_t immediate, uint32_t rs1, uint32_t funct3, uint32_t rd, uint32_t opcode);
/* uint32_t get_i_immediate(uint32_t instruction); */

int main() {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  /* uint64_t program_counter = 0; */

  /* printf("compiled with warning"); */

  uint32_t memory[MEMORY_SIZE] = {};

  for (uint32_t i = 0; i < MEMORY_SIZE; i++) {
    memory[i] = i;
  }

  uint32_t instructions[1] = {
    create_i_type(10, 10, LW, 10, LOAD),
  };

  for (int i = 0; i < 1; i++) {
    run_instruction(instructions[i], memory, registers);
  }

  /* for (int i = 0; i < MEMORY_SIZE; i++) { */
  /*   printf("mem addr %d: %" PRIu32 "\n", i,  memory); */
  /* } */

  print_memory_values(memory);
  print_register_values(registers);

  return 0;
}

/**
 * @brief creates an i-type instruction
 *
 * @param immediate - the immediate value
 * @param rs1 - register source 1
 * @param funct3 - code that determines specific instruction from opcode
 * @return
 **/
uint32_t create_i_type(uint32_t immediate,
                       uint32_t rs1,
                       uint32_t funct3,
                       uint32_t rd,
                       uint32_t opcode) {
  immediate = immediate << 20;
  rs1 = rs1 << 15;
  funct3 = funct3 << 12;
  rd = rd << 7;
  return immediate | rs1 | funct3 | rd | opcode;
}
