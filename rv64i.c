#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#define NUM_REGISTERS 32


void print_register_values(const uint64_t registers[NUM_REGISTERS]) {
  // add bounds check later
  
  for (int i = 0; i < NUM_REGISTERS; i++) {
    printf("register x%d = %" PRIu64 "\n", i, registers[i]);
  }
}

uint64_t stack_pointer(const uint64_t registers[NUM_REGISTERS]) {
  return registers[2];
}

uint64_t return_address(const uint64_t registers[NUM_REGISTERS]) {
  return registers[1];
}

uint64_t alternate_return_address(const uint64_t registers[NUM_REGISTERS]) {
  return registers[5];
}

enum months {
  LOAD = 0b0000011, LOAD_FP = 0b0000011, CUSTOM_0, MISC_MEM, OP_IMM, AUIPC, OP_IMM_32,
  STORE, STORE_FP, CUSTOM_1, AMO, OP, LUI, OP_32,
  MADD, MSUB, NMSUB, NMADD, OP_FP, OP_V, CUSTOM_2,
  BRANCH, JALR, RESERVED, JAL, SYSTEM, OP_VE, CUSTOM_3
};

int opcode_bits_1_0(uint32_t instruction) {
  return instruction & 0b11;
}

int opcode_bits_4_2(uint32_t instruction) {
  return instruction & 0b11100 >> 2;
}

int opcode_bits_6_5(uint32_t instruction) {
  return instruction & 0b1100000 >> 5;
}



/* int opcode_6_colon_5(uint32_t instruction) { */

/* } */

int main() {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint64_t program_counter = 0;

  print_register_values(registers);

  uint64_t cur_stack_pointer = stack_pointer(registers);
  uint64_t cur_return_address = return_address(registers);
  uint64_t cur_alternate_return_address = alternate_return_address(registers);

  printf("cur_stack_pointer: %" PRIu64 "\n", cur_stack_pointer);
  printf("cur_return_address: %" PRIu64 "\n", cur_return_address);
  printf("cur_alternate_return_address: %" PRIu64 "\n", cur_alternate_return_address);



  return 0;
}
