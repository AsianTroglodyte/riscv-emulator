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

enum Instruction_Enums {
  LOAD=     0b0000011,
  LOAD_FP=  0b0000111,
  CUSTOM_0= 0b0001011,
  MISC_MEM= 0b0001111,
  OP_IMM=   0b0010011,
  AUIPC=    0b0010111,
  OP_IMM_32=0b0011011,

  STORE=    0b0100011,
  STORE_FP= 0b0100111,
  CUSTOM_1= 0b0101011,
  AMO=      0b0101111,
  OP=       0b0110011,
  LUI=      0b0110111,
  OP_32=    0b0111011,

  MADD=     0b1000011,
  MSUB=     0b1000111,
  NMSUB=    0b1001011,
  NMADD=    0b1001111,
  OP_FP=    0b1010011,
  OP_V=     0b1010111,
  CUSTOM_2= 0b1011011,

  BRANCH=   0b1100011,
  JALR=     0b1100111,
  RESERVED= 0b1101011,
  JAL=      0b1101111,
  SYSTEM=   0b1110011,
  OP_VE=    0b1110111,
  CUSTOM_3= 0b1111011
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

int get_opcode(uint32_t instruction) {
  return instruction & 0b1111111;
}


int main() {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint64_t program_counter = 0;

  /* print_register_values(registers); */

  uint64_t cur_stack_pointer = stack_pointer(registers);
  uint64_t cur_return_address = return_address(registers);
  uint64_t cur_alternate_return_address = alternate_return_address(registers);

  printf("cur_stack_pointer: %" PRIu64 "\n", cur_stack_pointer);
  printf("cur_return_address: %" PRIu64 "\n", cur_return_address);
  printf("cur_alternate_return_address: %" PRIu64 "\n", cur_alternate_return_address);

  uint32_t instructions[4] = {
    LOAD,
    STORE,
    MADD,
    BRANCH
  };

  for (int i = 0; i < 4; i++) {
    uint32_t current_instruction = instructions[i];

    switch (get_opcode(current_instruction)) {
      case LOAD:
        printf("LOAD\n");
        break;
      case LOAD_FP:
        printf("LOAD_FP\n");
        break;
      case CUSTOM_0:
        printf("CUSTOM_0\n");
        break;
      case MISC_MEM:
        printf("MISC_MEM\n");
        break;
      case OP_IMM:
        printf("OP_IMM\n");
        break;
      case AUIPC:
        printf("AUIPC\n");
        break;
      case OP_IMM_32:
        printf("OP_IMM_32\n");
        break;

      case STORE:
        printf("STORE\n");
        break;
      case STORE_FP:
        printf("STORE_FP\n");
        break;
      case CUSTOM_1:
        printf("CUSTOM_1\n");
        break;
      case AMO:
        printf("AMO\n");
        break;
      case OP:
        printf("OP\n");
        break;
      case LUI:
        printf("LUI\n");
        break;
      case OP_32:
        printf("OP_32\n");
        break;

      case MADD:
        printf("MADD\n");
        break;
      case MSUB:
        printf("MSUB\n");
        break;
      case NMSUB:
        printf("NMSUB\n");
        break;
      case NMADD:
        printf("NMADD\n");
        break;
      case OP_FP:
        printf("OP_FP\n");
        break;
      case OP_V:
        printf("OP_V\n");
        break;
      case CUSTOM_2:
        printf("CUSTOM_2\n");
        break;

      case BRANCH:
        printf("BRANCH\n");
        break;
      case JALR:
        printf("JALR\n");
        break;
      case RESERVED:
        printf("RESERVED\n");
        break;
      case JAL:
        printf("JAL\n");
        break;
      case SYSTEM:
        printf("SYSTEM\n");
        break;
      case OP_VE:
        printf("OP_VE\n");
        break;
      case CUSTOM_3:
        printf("CUSTOM_3\n");
        break;
      default:
        printf("Couldn't determine instruction\n");
        break;
    }
  }

  return 0;
}
