#include "rv64i.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#define MEMORY_WORDS 32
#define MEMORY_BYTES MEMORY_WORD * 4

int main() {
  struct machine_state state = {
    .registers = {0, 1, 2, 0, 0, 5},
  };

  for (uint32_t i = 0; i < MEMORY_WORDS; i++) {
    state.memory[i] = i;
  }

  uint32_t instructions[1] = {
    create_i_type(10, 10, LW, 10, LOAD),
  };

  for (int i = 0; i < 1; i++) {
    run_instruction(instructions[i], &state);
  }

  print_memory(state.memory);
  print_registers(state.registers);

  return 0;
}

/**
 * @brief creates an i-type instruction
 *
 * @asdfasdf asdfsfd- the immediate value
 * @param rs1 - register source 1
 * @param funct3 - code that determines specific instruction from opcode
 * @return
 **/
