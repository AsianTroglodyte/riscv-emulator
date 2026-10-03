#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "rv64i.h"

int get_opcode(uint32_t instruction);
uint32_t get_rs1(uint32_t instruction);
uint32_t get_rs2(uint32_t instruction);
uint32_t get_rd(uint32_t instruction);
uint32_t get_funct3(uint32_t instruction);
uint32_t get_funct7(uint32_t instruction);
uint32_t get_i_immediate(uint32_t instruction);
void print_bits(const unsigned int num);

void run_instruction(uint32_t instruction,
                     uint32_t memory[],
                     uint64_t registers[]) {

  switch (get_opcode(instruction)) {
    uint32_t i_immediate = get_i_immediate(instruction);
    uint32_t rs1 = get_rs1(instruction);
    uint32_t rd =  get_rd(instruction);
    uint32_t funct3 = get_funct3(instruction);
  case LOAD:
    // I Type
    printf("LOAD\n");
    i_immediate = get_i_immediate(instruction);
    rs1 = get_rs1(instruction);
    rd =  get_rd(instruction);
    funct3 = get_funct3(instruction);

    uint32_t address = registers[rs1] + i_immediate;
    switch (funct3) {
    case LB:
      printf("LB\n");
      break;
    case LH:
      printf("LH\n");
      break;
    case LW:
      printf("LW\n");

      printf("instruction ");
      print_bits(instruction);

      printf("immediate ");
      print_bits(i_immediate);

      printf("rs1 " );
      print_bits(rs1);

      printf("rd ");
      print_bits(rd);

      printf("funct3 ");
      print_bits(funct3);

      registers[rd] = memory[address];
      break;
    case LBU:
      printf("LBU\n");
      break;
    case LHU:
      printf("LHU\n");
      break;
    default:
      printf("LOAD invalid func3: %d", funct3);
    }

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
    // Integer Register-Immediate Instructions
    // I-Type
    printf("OP_IMM\n");
    i_immediate = get_i_immediate(instruction);
    rs1 = get_rs1(instruction);
    rd =  get_rd(instruction);
    funct3 = get_funct3(instruction);


    /* switch (funct3) { */
    /*   match */
    /*   break; */
    /* } */
    /* break; */
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
    printf("Invalid Opcode\n");
    break;
  }
}

/**
 * @brief gets the bits from a particular 32 bit
 *
 * @param digit index (right to left) to start slicing from
 * @param the length of slice
 * @return uint32_t bit slice shifted so start is beginning digit.
 **/
static inline uint32_t bits(uint32_t instruction, int start, int end) {
  int length = end - start + 1;
  return (instruction >> start) & ((1u << length) - 1);
}

uint32_t get_rd(uint32_t instruction) {
  return bits(instruction, 7, 11);
}

uint32_t get_rs1(uint32_t instruction) {
  return bits(instruction, 15, 19);
}

uint32_t get_rs2(uint32_t instruction) {
  return bits(instruction, 20, 24);
}

uint32_t get_funct3(uint32_t instruction) {
  return bits(instruction, 12, 14);
}

uint32_t get_funct7(uint32_t instruction) {
  return bits(instruction, 25, 31);
}

uint32_t get_i_immediate(uint32_t instruction) {
  uint32_t imm_11_0 = bits(instruction, 20, 31);
  return imm_11_0;
}

uint32_t get_s_immediate(uint32_t instruction) {
  uint32_t imm_11_5 = bits(instruction, 25, 31) << 5;
  uint32_t imm_4_0 = bits(instruction, 7, 11);
  return imm_11_5 + imm_4_0;
}

uint32_t get_b_immediate(uint32_t instruction) {
  uint32_t imm_12 = bits(instruction, 31, 31) << 12;
  uint32_t imm_10_5 = bits(instruction, 25, 30) << 5;
  uint32_t imm_4_1 = bits(instruction, 8, 11) << 1;
  uint32_t imm_11 = bits(instruction, 7, 7) << 11;
  return (imm_12 + imm_11 + imm_10_5 + imm_4_1);
}

uint32_t get_u_immediate(uint32_t instruction) {
  uint32_t imm_31_12 = bits(instruction, 12, 31) << 12;
  return imm_31_12;
}

int j_immediate(uint32_t instruction) {
  uint32_t imm_20 = bits(instruction, 31, 31) << 20;
  uint32_t imm_10_1 = bits(instruction, 21, 30) << 1;
  uint32_t imm_11 = bits(instruction, 20, 20) << 11;
  uint32_t imm_19_12 = bits(instruction, 12, 19) << 12;
  return imm_20 + imm_10_1 + imm_11 + imm_19_12;
}

void print_register_values(const uint64_t registers[NUM_REGISTERS]) {
  // add bounds check later
  for (int i = 0; i < NUM_REGISTERS; i++) {
    printf("register x%d = %" PRIu64 "\n", i, registers[i]);
  }
}

void print_memory_values(const uint32_t memory[]) {
  for (int i = 0; i < NUM_REGISTERS; i++) {
    printf("memory addr %d = %" PRIu32 "\n", i, memory[i]);
  }
}

void print_bits(const unsigned int num) {
  int total_bits = sizeof(num) * 8;

  for (int i = total_bits - 1; i >= 0; i--) {
    int bit = (num >> i) & 1;
    printf("%d", bit);
    if (i % 4 == 0)  printf(" ");
  }

  printf("\n");
}

int opcode_bits_1_0(uint32_t instruction) {
  return instruction & 0b11;
};

int opcode_bits_4_2(uint32_t instruction) {
  return instruction & 0b11100 >> 2;
}

int opcode_bits_6_5(uint32_t instruction) {
  return instruction & 0b1100000 >> 5;
}

int get_opcode(uint32_t instruction) {
  return instruction & 0b1111111;
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
