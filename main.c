#include "rv64i.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int main() {
  /* uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5}; */
  /* uint64_t program_counter = 0; */


  uint32_t instructions[4] = {
    LUI,
    LOAD,
    MADD,
    BRANCH
  };



  for (int i = 0; i < 4; i++) {
    run_instruction(instructions[i]);
  }

  return 0;
}
