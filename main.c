#include "rv64i.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#define MEMORY_WORDS 32
#define MEMORY_BYTES MEMORY_WORD * 4

int main() {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_WORDS] = {};

  for (uint32_t i = 0; i < MEMORY_WORDS; i++) {
    memory[i] = i;
  }

  uint32_t instructions[1] = {
    create_i_type(10, 10, LW, 10, LOAD),
  };

  for (int i = 0; i < 1; i++) {
    run_instruction(instructions[i], memory, registers);
  }

  print_memory(memory);
  print_registers(registers);

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
