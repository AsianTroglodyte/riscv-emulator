#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "rv64i.h"
#include <assert.h>

void run_instruction(uint32_t instruction,
                     uint8_t memory[],
                     uint64_t registers[]) {

  switch (get_opcode(instruction)) {
    int32_t immediate;
    uint32_t rs1_index;
    uint32_t rs2_index;
    uint32_t rd_index;
    uint32_t funct3;
    uint64_t value;

    uint32_t address;
  case LOAD:
    // I Type
    immediate = get_i_immediate(instruction);
    rs1_index = get_rs1_index(instruction);
    rd_index =  get_rd_index(instruction);
    funct3 = get_funct3(instruction);

    address = registers[rs1_index] + immediate;
    switch (funct3) {
    case LB:
      registers[rd_index] = (int8_t) get_byte(memory, address);
      break;
    case LH:
      registers[rd_index] = (int16_t) get_half_word(memory, address);
      break;
    case LW:
      registers[rd_index] = (int32_t) get_word(memory, address);
      break;
    case LBU:
      registers[rd_index] = get_byte(memory, address);
      break;
    case LHU:
      registers[rd_index] = get_half_word(memory, address);
      break;
    default:
      printf("funct3 %d does not correspond to any LOAD instruction.", funct3);
      assert(0);
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
    immediate = get_i_immediate(instruction);
    rs1_index = get_rs1_index(instruction);
    rd_index = get_rd_index(instruction);
    funct3 = get_funct3(instruction);

    switch (funct3) {
      uint32_t shamt;
      uint32_t immediate_11_5;
    case ADDI:
      registers[rd_index] = registers[rs1_index] + immediate;
      break;
    case SLTI:
      registers[rd_index] = (int64_t) registers[rs1_index] < immediate;
      break;
    case SLTIU:
      registers[rd_index] = registers[rs1_index] < (uint64_t) immediate;
      break;
    case ORI:
      registers[rd_index] = registers[rs1_index] | immediate;
      break;
    case ANDI:
      registers[rd_index] = registers[rs1_index] & immediate;
      break;
    case SLLI:
      shamt = bits(immediate, 0, 5);
      registers[rd_index] = registers[rs1_index] << shamt;
      break;
    case SRLIorSRAI:
      shamt = bits(immediate, 0, 5);
      immediate_11_5 = bits(immediate, 6, 11);
      switch (immediate_11_5) {
      case SRLI_IMM:
        registers[rd_index] = registers[rs1_index] >> shamt;
        break;
      case SRAI_IMM: {
        uint64_t result = registers[rs1_index] >> shamt;
        if (registers[rs1_index] & UINT64_C(1) << 63) {
          result |= (UINT64_MAX << (64 - shamt));
        }
        registers[rd_index] = result;
        break;
      }
      default:
        print_bits(instruction);
        printf("funct3 %d and imm_11_5 %d does not correspond to any OP_IMM instruction.\n",
               funct3, immediate_11_5);
      }
      break;
    default:
      printf("funct3 %d does not correspond to any OP_IMM instruction.\n", funct3);
      assert(0);
    }

    switch (funct3) {

    }
    break;
  case AUIPC:
    printf("AUIPC\n");
    break;

  case OP_IMM_32:
    printf("OP_IMM_32");
    break;
  case STORE:
    immediate = get_s_immediate(instruction);
    rs1_index = get_rs1_index(instruction);
    rs2_index =  get_rs2_index(instruction);
    funct3 = get_funct3(instruction);

    address = registers[rs1_index] + immediate;
    switch (funct3) {
    case SB:
      write_byte(memory, address, registers[rs2_index]);
      break;
    case SH:
      write_half_word(memory, address, registers[rs2_index]);
      break;
    case SW:
      write_word(memory, address, registers[rs2_index]);
      break;
    case SD:
      write_double_word(memory, address, registers[rs2_index]);
      break;
    default:
      printf("funct3 %d does not correspond to any STORE instruction.", funct3);
      assert(0);
    }
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
    rs1_index = get_rs1_index(instruction);
    rs2_index =  get_rs2_index(instruction);
    rd_index = get_rd_index(instruction);
    uint32_t funct7_funct3 = (get_funct7(instruction) << 3) | get_funct3(instruction);
    switch (funct7_funct3) {
    case ADD:
      registers[rd_index] = registers[rs1_index] + registers[rs2_index];
      break;
    case SUB:
      registers[rd_index] = registers[rs1_index] - registers[rs2_index];
      break;
    case SLL:
      registers[rd_index] = registers[rs1_index] << registers[rs2_index];
      break;
    case SLT:
      registers[rd_index] = (int64_t) registers[rs1_index] < (int64_t) registers[rs2_index];
      break;
    case SLTU:
      registers[rd_index] = registers[rs1_index] < registers[rs2_index];
      break;
    case XOR:
      registers[rd_index] = registers[rs1_index] ^ registers[rs2_index];
      break;
    case SRL:
      registers[rd_index] = registers[rs1_index] >> registers[rs2_index];
      break;
    case SRA:
      registers[rd_index] = (int64_t) registers[rs1_index] >> registers[rs2_index];
      break;
    case OR:
      printf("OR");
      break;
    case AND:
      printf("AND");
      break;
    default:
      printf("funct7_funct3  %d does not correspond to any OP instruction.", funct7_funct3);
      assert(0);
    }
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
inline uint32_t bits(uint32_t instruction, int start, int end) {
  int length = end - start + 1;
  return (instruction >> start) & ((1u << length) - 1);
}

uint32_t get_rd_index(uint32_t instruction) {
  return bits(instruction, 7, 11);
}

uint32_t get_rs1_index(uint32_t instruction) {
  return bits(instruction, 15, 19);
}

uint32_t get_rs2_index(uint32_t instruction) {
  return bits(instruction, 20, 24);
}

uint32_t get_funct3(uint32_t instruction) {
  return bits(instruction, 12, 14);
}

uint32_t get_funct7(uint32_t instruction) {
  return bits(instruction, 25, 31);
}

int32_t get_i_immediate(uint32_t instruction) {
  int32_t imm_11_0 = (int32_t) sign_extend_32(bits(instruction, 20, 31), 12);
  return imm_11_0;
}

int32_t get_s_immediate(uint32_t instruction) {
  uint32_t immediate_4_0 = bits(instruction, 7, 11);
  uint32_t immediate_11_5 = bits(instruction, 25, 31) << 5;
  uint32_t immediate_11_0 = immediate_4_0 + immediate_11_5;
  int32_t imm_11_0 = (int32_t) sign_extend_32(immediate_11_0, 12);
  return imm_11_0;
}

int32_t sign_extend_32(uint32_t field, int width) {
  assert(width >= 1 && width <= 32);
  uint32_t mask = UINT32_MAX >> (32 - width);
  uint32_t sign_bit = UINT32_C(1) << (width - 1);
  field &= mask;

  // check if signage of number is negative
  if (field & sign_bit) {
    return (int64_t)field - (INT64_C(1) << width);
  }

  return field;
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

void print_registers(const uint64_t registers[NUM_REGISTERS]) {
  // add bounds check later
  for (int i = 0; i < NUM_REGISTERS; i++) {
    printf("register x%d = %" PRIu64 "\n", i, registers[i]);
  }
}

void print_memory(const uint8_t memory[]) {
  int byte_address = 0;
  for (int i = 0; i < MEMORY_WORDS; i++) {
    byte_address = i * 4;
    printf("memory addr %d-%d = ", byte_address + 3, byte_address);
    print_bits(get_word(memory, byte_address));
  }
}

void print_bits(const size_t num) {
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


// GET DATA FROM MEMORY GIVEN AN ADDRESS
inline uint64_t get_double_word(uint8_t const memory[], uint32_t address) {
  return ((uint64_t)memory[address]) |
         ((uint64_t)memory[address + 1]) << 8 |
         ((uint64_t)memory[address + 2]) << 16 |
         ((uint64_t)memory[address + 3]) << 24 |
         ((uint64_t)memory[address + 4]) << 32 |
         ((uint64_t)memory[address + 5]) << 40 |
         ((uint64_t)memory[address + 6]) << 48 |
         ((uint64_t)memory[address + 7]) << 56 ;
}

inline uint32_t get_word(uint8_t const memory[], uint32_t address) {
  return ((uint32_t)memory[address]) |
         ((uint32_t)memory[address + 1]) << 8  |
         ((uint32_t)memory[address + 2]) << 16 |
         ((uint32_t)memory[address + 3]) << 24;
}

inline uint16_t get_half_word(uint8_t const memory[], uint32_t address) {
  return (uint16_t)memory[address] |
         (uint16_t)memory[address + 1] << 8;
}

inline uint8_t get_byte(uint8_t const memory[], uint32_t address) {
  return memory[address];
}

// WRITE DATA TO MEMORY GIVEN AN ADDRESS AND VALUE
void write_double_word(uint8_t memory[], uint32_t address, uint64_t field) {
  for (unsigned i = 0; i < 8; ++i) {
    memory[address + i] = (uint8_t)(field >> (i * 8));
  }
}

void write_word(uint8_t memory[], uint32_t address, uint64_t value) {
  for (unsigned i = 0; i < 4; ++i) {
    memory[address + i] = (uint8_t)(value >> (i * 8));
  }
}

void write_half_word(uint8_t memory[], uint32_t address, uint64_t value) {
  for (unsigned i = 0; i < 2; ++i) {
    memory[address + i] = (uint8_t)(value >> (i * 8));
  }
}

void write_byte(uint8_t memory[], uint32_t address, uint64_t value) {
  memory[address ] = value;
}


static void assert_valid_opcode(int32_t opcode) {
  assert((opcode >= 0 && opcode <= 0b1111111 )
         && "opcode must be a within 0-127");
}
static void assert_valid_register(int32_t rs1_index) {
  assert((rs1_index >= 0 && rs1_index < NUM_REGISTERS )
         && "rs1_index must be a valid register index");
}

static void assert_valid_funct3(int32_t funct3) {
    assert((funct3 >= 0 && funct3 <= 0b111 )
         && "funct3 code must be within 0-7");
}

// INSTRUCTION CREATION FILE
uint32_t create_i_type(uint32_t immediate,
                       int32_t rs1_index,
                       int32_t funct3,
                       int32_t rd_index,
                       int32_t opcode) {
  assert_valid_register(rs1_index);
  assert_valid_funct3(funct3);
  assert_valid_register(rd_index);
  assert_valid_opcode(opcode);

  immediate = immediate << 20;
  rs1_index = rs1_index << 15;
  funct3 = funct3 << 12;
  rd_index = rd_index << 7;
  return (uint32_t) immediate | rs1_index | funct3 | rd_index | opcode;
}

uint32_t create_s_type(uint32_t immediate,
                       int32_t rs2_index,
                       int32_t rs1_index,
                       int32_t funct3,
                       int32_t opcode) {
  assert_valid_register(rs2_index);
  assert_valid_register(rs1_index);
  assert_valid_funct3(funct3);
  assert_valid_opcode(opcode);

  uint32_t immediate_4_0 = bits(immediate, 0, 4) << 7;
  uint32_t immediate_11_5 = bits(immediate, 5, 11) << 25;
  rs2_index = rs2_index << 20;
  rs1_index = rs1_index << 15;
  funct3 = funct3 << 12;
  return (uint32_t) immediate_11_5 | rs2_index | rs1_index | funct3 | immediate_4_0 | opcode;
}

uint32_t create_r_type(uint32_t funct7_funct3,
                       int32_t rs2_index,
                       int32_t rs1_index,
                       int32_t rd_index,
                       int32_t opcode) {
  assert_valid_register(rs2_index);
  assert_valid_register(rs1_index);
  assert_valid_opcode(opcode);
  assert_valid_funct3(funct7_funct3 & UINT32_C(0b111));

  uint32_t funct3 = (funct7_funct3 & UINT32_C(0b111)) << 12;
  uint32_t funct7 = (funct7_funct3 >> 3) << 25;
  rs2_index = rs2_index << 20;
  rs1_index = rs1_index << 15;
  rd_index = rd_index << 7;

  return (uint32_t) funct7 | rs2_index | rs1_index | funct3 | rd_index | opcode;
}
