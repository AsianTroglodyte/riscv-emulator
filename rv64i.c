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

enum instructions {
  LOAD=0b0000011,
  LOAD_FP=0b0000111,
  CUSTOM_0=0b0001011,
  MISC_MEM=0b0001111,
  OP_IMM=0b0010011,
  AUIPC=0b0010111,
  OP_IMM_32=0b0011011,

  STORE=0b0100011,
  STORE_FP=0b0100100011,
  CUSTOM_1=0b0101000111,
  AMO=0b0101101011,
  OP=0b0110001111,
  LUI=0b0110110011,
  OP_32=0b0111011,

  MADD=0b1000011,
  MSUB=0b1000111,
  NMSUB=0b1001011,
  NMADD=0b1001111,
  OP_FP=0b1010011,
  OP_V=0b1010111,
  CUSTOM_2=0b1011011,

  BRANCH=0b1100011,
  JALR=0b1100111,
  RESERVED=0b1101011,
  JAL=0b1101111,
  SYSTEM=0b1110011,
  OP_VE=0b1110111,
  CUSTOM_3=0b1111011
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
