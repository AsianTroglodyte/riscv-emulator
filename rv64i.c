#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "rv64i.h"
#include <assert.h>
#include <stdlib.h>


void run_instruction(uint32_t instruction,
                    struct machine_state *machine_state) {
  uint8_t *memory = machine_state->memory;
  uint64_t *registers = machine_state->registers;
  uint64_t *program_counter = &machine_state->program_counter;
  assert((*program_counter % 4) == 0);
  assert(registers[0] == 0);

  // program_counter set to next_pc at end
  // default value here overrided by JUMP and BRANCH instructions if needed.
  int64_t next_pc = *program_counter + 4;

  switch (get_opcode(instruction)) {
    int32_t immediate;
    uint32_t rs1_index;
    uint32_t rs2_index;
    uint32_t rd_index;
    uint32_t funct3;
    uint32_t funct7;

    uint64_t address;
    uint32_t immediate_id;

  case OPCODE_LOAD:
    // I Type
    immediate = get_i_immediate(instruction);
    rs1_index = get_rs1_index(instruction);
    rd_index =  get_rd_index(instruction);
    funct3 = get_funct3(instruction);

    address = (int64_t) registers[rs1_index] + immediate;
    switch (funct3) {
    case FUNCT3_LB:
      registers[rd_index] = (int8_t) get_byte(memory, address);
      break;
    case FUNCT3_LH:
      registers[rd_index] = (int16_t) get_half_word(memory, address);
      break;
    case FUNCT3_LW:
      registers[rd_index] = (int32_t) get_word(memory, address);
      break;
    case FUNCT3_LBU:
      registers[rd_index] = get_byte(memory, address);
      break;
    case FUNCT3_LHU:
      registers[rd_index] = get_half_word(memory, address);
      break;
    case FUNCT3_LWU:
      registers[rd_index] = get_word(memory, address);
      break;
    case FUNCT3_LD:
      registers[rd_index] = get_double_word(memory, address);
      break;
    default:
      printf("funct3 %d does not correspond to any LOAD instruction.", funct3);
      assert(0);
    }
    break;
  case OPCODE_LOAD_FP:
    printf("LOAD_FP\n");
    break;
  case OPCODE_CUSTOM_0:
    printf("CUSTOM_0\n");
    break;
  case OPCODE_MISC_MEM:
    /* if  */
    printf("MISC_MEM\n");
    break;
  case OPCODE_OP_IMM:
    // Integer Register-Immediate Instructions
    // I-Type
    immediate = get_i_immediate(instruction);
    rs1_index = get_rs1_index(instruction);
    rd_index = get_rd_index(instruction);
    funct3 = get_funct3(instruction);

    switch (funct3) {
      uint32_t shamt;
    case FUNCT3_ADDI:
      registers[rd_index] = registers[rs1_index] + immediate;
      break;
    case FUNCT3_SLTI:
      registers[rd_index] = (int64_t) registers[rs1_index] < immediate;
      break;
    case FUNCT3_SLTIU:
      registers[rd_index] = registers[rs1_index] < (uint64_t) immediate;
      break;
    case FUNCT3_ORI:
      registers[rd_index] = registers[rs1_index] | immediate;
      break;
    case FUNCT3_ANDI:
      registers[rd_index] = registers[rs1_index] & immediate;
      break;
    case FUNCT3_SLLI:
      if (bits(instruction, 26, 31) == IMM_SLLI) {
        shamt = bits(immediate, 0, 5);
        registers[rd_index] = registers[rs1_index] << shamt;
      } else {
        /* assert(1 & printf("immediate of potential SLLI is not ")); */
        printf("immediate of potential SLLI is not \n");
      }
      break;
    case FUNCT3_XORI:
      registers[rd_index] = registers[rs1_index] ^ immediate;
      break;
    case FUNCT3_SHIFT_RIGHT_OPP:
      shamt = bits(immediate, 0, 5);
      uint32_t funct6 = bits(immediate, 6, 11);
      switch (funct6) {
      case IMM_SRLI:
        registers[rd_index] = registers[rs1_index] >> shamt;
        break;
      case IMM_SRAI: {
        uint64_t result = registers[rs1_index] >> shamt;
        bool is_negative = (registers[rs1_index] & (UINT64_C(1) << 63)) != 0;
        if (shamt != 0 && is_negative) {
          result |= (UINT64_MAX << (64 - shamt));
        }
        registers[rd_index] = result;
        break;
      }
      default:
        print_bits(instruction);
        printf("funct3 %d and funct6 %d does not correspond to any OP_IMM instruction.\n",
               funct3, funct6);
      }
      break;
    default:
      printf("funct3 %d does not correspond to any OP_IMM instruction.\n", funct3);
      assert(0);
    }
    break;
  case OPCODE_AUIPC:
    rd_index = get_rd_index(instruction);
    registers[rd_index] = (uint64_t)(int64_t)get_upper_immediate(instruction) + *program_counter;
    break;
  case OPCODE_OP_IMM_32:
    rs1_index = get_rs1_index(instruction);
    rd_index = get_rd_index(instruction);
    funct3 = get_funct3(instruction);
    immediate = get_i_immediate(instruction);
    // ADDIW can be determined using juts func3
    switch (funct3) {
    case FUNCT3_ADDIW:
      registers[rd_index] = (uint32_t) registers[rs1_index] + immediate;
      break;
    case FUNCT3_SLLIW:
      immediate_id = get_funct7(instruction);
      if (immediate_id == IMM_SLLIW) {
        uint32_t shamt = bits(immediate, 0, 4);
        registers[rd_index] = (uint32_t) registers[rs1_index] << shamt;
      }
      else {
        printf("SLLIW_FUNCT_3 does not have correct corresponding immediate_id/funct7 val");
      }
      break;
    case FUNCT3_SHIFT_RIGHT_OPP_32:
      uint32_t shamt = bits(immediate, 0, 4);
      immediate_id = get_funct7(instruction);
      switch (immediate_id) {
      case (IMM_SRLIW):
        registers[rd_index] = (uint32_t)registers[rs1_index] >> shamt;
        break;
      case (IMM_SRAIW):
        registers[rd_index] = (int64_t)(int32_t)registers[rs1_index] >> shamt;
        break;
      }
    }
    break;
  case OPCODE_STORE:
    immediate = get_s_immediate(instruction);
    rs1_index = get_rs1_index(instruction);
    rs2_index =  get_rs2_index(instruction);
    funct3 = get_funct3(instruction);

    address = registers[rs1_index] + immediate;
    switch (funct3) {
    case FUNCT3_SB:
      write_byte(memory, address, registers[rs2_index]);
      break;
    case FUNCT3_SH:
      write_half_word(memory, address, registers[rs2_index]);
      break;
    case FUNCT3_SW:
      write_word(memory, address, registers[rs2_index]);
      break;
    case FUNCT3_SD:
      write_double_word(memory, address, registers[rs2_index]);
      break;
    default:
      printf("funct3 %d does not correspond to any STORE instruction.", funct3);
      assert(0);
    }
    break;
  case OPCODE_STORE_FP:
    printf("STORE_FP\n");
    break;
  case OPCODE_CUSTOM_1:
    printf("CUSTOM_1\n");
    break;
  case OPCODE_AMO:
    printf("AMO\n");
    break;
  case OPCODE_OP: {

    rs1_index = get_rs1_index(instruction);
    rs2_index =  get_rs2_index(instruction);
    rd_index = get_rd_index(instruction);
    uint32_t funct7_funct3 = (get_funct7(instruction) << 3) | get_funct3(instruction);
    switch (funct7_funct3) {
    case (FUNCT7_ADD << 3) | FUNCT3_ADD:
      registers[rd_index] = registers[rs1_index] + registers[rs2_index];
      break;
    case (FUNCT7_SUB << 3) | FUNCT3_SUB:
      registers[rd_index] = registers[rs1_index] - registers[rs2_index];
      break;
    case (FUNCT7_SLL << 3) | FUNCT3_SLL:
      // we only use lowest 6 bits of register value to shift
      registers[rd_index] = registers[rs1_index] << (registers[rs2_index] & 0b111111);
      break;
    case (FUNCT7_SLT << 3) | FUNCT3_SLT:
      registers[rd_index] = (int64_t) registers[rs1_index] < (int64_t) registers[rs2_index];
      break;
    case (FUNCT7_SLTU << 3) | FUNCT3_SLTU:
      registers[rd_index] = registers[rs1_index] < registers[rs2_index];
      break;
    case (FUNCT7_XOR << 3) | FUNCT3_XOR:
      registers[rd_index] = registers[rs1_index] ^ registers[rs2_index];
      break;
    case (FUNCT7_SRL << 3) | FUNCT3_SRL:
      // we only use lowest 6 bits of register value to shift
      registers[rd_index] = registers[rs1_index] >> (registers[rs2_index] & 0b111111);
      break;
    case (FUNCT7_SRA << 3) | FUNCT3_SRA:
      // we only use lowest 6 bits of register value to shift
      registers[rd_index] = (int64_t) registers[rs1_index] >> (registers[rs2_index] & 0b111111);
      break;
    case (FUNCT7_OR << 3) | FUNCT3_OR:
      registers[rd_index] = (int64_t) registers[rs1_index] | registers[rs2_index];
      break;
    case (FUNCT7_AND << 3) | FUNCT3_AND:
      registers[rd_index] = (int64_t) registers[rs1_index] & registers[rs2_index];
      break;
    default:
      printf("funct7_funct3  %d does not correspond to any OP instruction.", funct7_funct3);
      assert(0);
    }
    break;
  }
  case OPCODE_LUI:
    rd_index = get_rd_index(instruction);
    // NOTE: upper immediate is ONLY understood as a
    registers[rd_index] = get_upper_immediate(instruction);
    break;
  case OPCODE_OP_32:
    funct3 = get_funct3(instruction);
    funct7 = get_funct7(instruction);
    rs1_index = get_rs1_index(instruction);
    rs2_index = get_rs2_index(instruction);
    rd_index = get_rd_index(instruction);
    switch (funct3) {
    case FUNCT3_SLLW:
      if (FUNCT7_SLLW == funct7) {
        registers[rd_index] = (uint32_t) registers[rs1_index] << bits64(registers[rs2_index], 0, 4);
      }
      else {
        printf("funct7 not valid given SLLW_FUNCT_3\n");
      }
      break;
    case FUNCT3_ADDW_SUBW:
      if (FUNCT7_ADDW == funct7) {
        registers[rd_index] = (int64_t)(int32_t)((uint32_t) registers[rs1_index] + (uint32_t) registers[rs2_index]);
      }
      else if (FUNCT7_SUBW == funct7) {
        registers[rd_index] = (int64_t)(int32_t)((uint32_t) registers[rs1_index] - (uint32_t) registers[rs2_index]);
      }
      else {
        printf("immediate ID given ADDW_SUBW_FUNCT_3 not valid\n");
      }
      break;
    case FUNCT3_SHIFT_RIGHT:
      if (FUNCT7_SRLW == funct7) {
        registers[rd_index] = (uint32_t)registers[rs1_index] >> (registers[rs2_index] & 0b11111);
      } else if (FUNCT7_SRAW == funct7) {
        registers[rd_index] = (int32_t)(uint32_t)registers[rs1_index] >> (registers[rs2_index] & 0b11111);
      } else {
        printf("immediate ID SHIFT_RIGHT_FUNCT_3 not valid\n");
      }
      break;
    }
    break;
  case OPCODE_MADD:
    printf("MADD\n");
    break;
  case OPCODE_MSUB:
    printf("MSUB\n");
    break;
  case OPCODE_NMSUB:
    printf("NMSUB\n");
    break;
  case OPCODE_NMADD:
    printf("NMADD\n");
    break;
  case OPCODE_OP_FP:
    printf("OP_FP\n");
    break;
  case OPCODE_OP_V:
    printf("OP_V\n");
    break;
  case OPCODE_CUSTOM_2:
    printf("CUSTOM_2\n");
    break;

  case OPCODE_BRANCH:
    rs2_index = get_rs2_index(instruction);
    rs1_index = get_rs1_index(instruction);
    immediate = get_b_immediate(instruction);
    funct3 = get_funct3(instruction);
    switch(funct3) {
    case FUNCT3_BEQ:
      if (registers[rs2_index] == registers[rs1_index]) {
        next_pc = *program_counter + immediate;
        break;
      }
      break;
    case FUNCT3_BNE:
      if (registers[rs2_index] != registers[rs1_index]) {
        next_pc = *program_counter + immediate;
        break;
      }
      break;
    case FUNCT3_BLT:
      if ((int64_t)registers[rs1_index] < (int64_t)registers[rs2_index]) {
        next_pc = *program_counter + immediate;
        break;
      }
      break;
    case FUNCT3_BGE:
      if ((int64_t)registers[rs1_index] >= (int64_t)registers[rs2_index]) {
        next_pc = *program_counter + immediate;
        break;
      }
      break;
    case FUNCT3_BLTU:
      if ((uint64_t)registers[rs1_index] < (uint64_t)registers[rs2_index]) {
        next_pc = *program_counter + immediate;
        break;
      }
      break;
    case FUNCT3_BGEU:
      if ((uint64_t)registers[rs1_index] >= (uint64_t)registers[rs2_index]) {
        next_pc = *program_counter + immediate;
        break;
      }
      break;
    }
    break;
  case OPCODE_JALR:
    rd_index = get_rd_index(instruction);
    rs1_index = get_rs1_index(instruction);
    // save address of next instruction
    registers[rd_index] = next_pc;
    // jump to target address
    next_pc = (int64_t) get_i_immediate(instruction) + registers[rs1_index];
    break;
  case OPCODE_RESERVED:
    printf("RESERVED\n");
    break;
  case OPCODE_JAL:
    rd_index = get_rd_index(instruction);
    // save address of next instruction
    registers[rd_index] = next_pc;
    // jump to target address
    /* int64_t immediate_value = (uint32_t) get_j_immediate(instruction) << 1; */
    next_pc = (int64_t) *program_counter + get_j_immediate(instruction);
    break;
  case OPCODE_SYSTEM:
    printf("SYSTEM\n");
    break;
  case OPCODE_OP_VE:
    printf("OP_VE\n");
    break;
  case OPCODE_CUSTOM_3:
    printf("CUSTOM_3\n");
    break;
  default:
    printf("Invalid Opcode\n");
    break;
  }

  *program_counter = next_pc;
}

/**
 * @brief gets the bits from a particular 32 bit
 *
 * @param digit index (right to left) to start slicing from
 * @param the length of slice
 * @return uint32_t bit slice shifted so start is beginning digit.
 **/
inline uint32_t bits(uint32_t bits, int start, int end) {
  assert(start <= end && end < 32);
  return (bits >> start) & ((1u << (end - start + 1)) - 1);
}

inline uint64_t bits64(uint64_t bits, int start, int end) {
  assert(start <= end && end < 64);
  return (bits >> start) & (((uint64_t)1 << (end - start + 1)) - 1);
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

int32_t get_j_immediate(uint32_t instruction) {
  uint32_t imm_20 = bits(instruction, 31, 31) << 20;
  uint32_t imm_10_1 = bits(instruction, 21, 30) << 1;
  uint32_t imm_11 = bits(instruction, 20, 20) << 11;
  uint32_t imm_19_12 = bits(instruction, 12, 19) << 12;
  uint32_t immediate = sign_extend_32(imm_20 + imm_10_1 + imm_11 + imm_19_12, 20);
  return immediate;
}

uint32_t get_u_immediate(uint32_t instruction) {
  uint32_t imm_31_12 = bits(instruction, 12, 31) << 12;
  return imm_31_12;
}

uint32_t get_b_immediate(uint32_t instruction) {
  uint32_t imm_12 = bits(instruction, 31, 31) << 12;
  uint32_t imm_10_5 = bits(instruction, 25, 30) << 5;
  uint32_t imm_4_1 = bits(instruction, 8, 11) << 1;
  uint32_t imm_11 = bits(instruction, 7, 7) << 11;

  return sign_extend_32((imm_12 + imm_11 + imm_10_5 + imm_4_1), 12);
}

// function works on U-Type (u for upper-immediate) AND regular immediate values.
int32_t get_upper_immediate(uint32_t instruction_or_immediate) {
  return instruction_or_immediate & (UINT32_MAX << 12);
}

uint32_t sign_extend_32(uint32_t field, int width) {
  assert(width >= 1 && width <= 32);
  uint32_t mask = UINT32_MAX >> (32 - width);
  uint32_t sign_bit = UINT32_C(1) << (width - 1);
  field &= mask;

  // check if signage of number is negative
  if (field & sign_bit) {
    return (int32_t)field - (INT32_C(1) << width);
  }

  return field;
}

uint64_t sign_extend_64(uint64_t field, int width) {
  assert(width >= 1 && width <= 64);
  uint64_t mask = UINT64_MAX >> (64 - width);
  uint64_t sign_bit = UINT64_C(1) << (width - 1);
  field &= mask;

  // check if signage of number is negative
  if (field & sign_bit) {
    return (int64_t)field - (INT64_C(1) << width);
  }

  return field;
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

void print_bits_impl(uint64_t value, unsigned width) {
  for (unsigned i = width; i > 0; --i) {
    putchar('0' + ((value >> (i - 1)) & UINT64_C(1)));
    /* printf("%" PRIu64, (UINT64_MAX - 1) & value); */
    if (((i - 1) % 4) == 0) putchar(' ');
  }
  putchar('\n');
}

inline int opcode_bits_1_0(uint32_t instruction) {
  return instruction & 0b11;
};

inline int opcode_bits_4_2(uint32_t instruction) {
  return instruction & 0b11100 >> 2;
}

inline int opcode_bits_6_5(uint32_t instruction) {
  return instruction & 0b1100000 >> 5;
}

inline int get_opcode(uint32_t instruction) {
  return instruction & 0b1111111;
}

inline uint64_t stack_pointer(const uint64_t registers[NUM_REGISTERS]) {
  return registers[2];
}

inline uint64_t return_address(const uint64_t registers[NUM_REGISTERS]) {
  return registers[1];
}

inline uint64_t alternate_return_address(const uint64_t registers[NUM_REGISTERS]) {
  return registers[5];
}


// GET DATA FROM MEMORY GIVEN AN ADDRESS
inline uint64_t get_double_word(uint8_t const memory[], uint64_t address) {
  return ((uint64_t)memory[address]) |
         ((uint64_t)memory[address + 1]) << 8 |
         ((uint64_t)memory[address + 2]) << 16 |
         ((uint64_t)memory[address + 3]) << 24 |
         ((uint64_t)memory[address + 4]) << 32 |
         ((uint64_t)memory[address + 5]) << 40 |
         ((uint64_t)memory[address + 6]) << 48 |
         ((uint64_t)memory[address + 7]) << 56 ;
}

inline uint32_t get_word(uint8_t const memory[], uint64_t address) {
  return ((uint32_t)memory[address]) |
         ((uint32_t)memory[address + 1]) << 8  |
         ((uint32_t)memory[address + 2]) << 16 |
         ((uint32_t)memory[address + 3]) << 24;
}

inline uint16_t get_half_word(uint8_t const memory[], uint64_t address) {
  return (uint16_t)memory[address] |
         (uint16_t)memory[address + 1] << 8;
}

inline uint8_t get_byte(uint8_t const memory[], uint64_t address) {
  return memory[address];
}

// WRITE DATA TO MEMORY GIVEN AN ADDRESS AND VALUE
void write_double_word(uint8_t memory[], uint64_t address, uint64_t field) {
  for (unsigned i = 0; i < 8; ++i) {
    memory[address + i] = (uint8_t)(field >> (i * 8));
  }
}

void write_word(uint8_t memory[], uint64_t address, uint64_t value) {
  for (unsigned i = 0; i < 4; ++i) {
    memory[address + i] = (uint8_t)(value >> (i * 8));
  }
}

void write_half_word(uint8_t memory[], uint64_t address, uint64_t value) {
  for (unsigned i = 0; i < 2; ++i) {
    memory[address + i] = (uint8_t)(value >> (i * 8));
  }
}

void write_byte(uint8_t memory[], uint64_t address, uint64_t value) {
  memory[address] = (uint8_t) value;
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


uint32_t create_i_type_shamt(uint32_t shamt,
                             int32_t rs1_index,
                             int32_t rd_index,
                             int32_t opcode,
                             int32_t funct3,
                             uint32_t upper_imm) {
  uint32_t shamt_bits;

  switch (opcode) {
  case OPCODE_OP_IMM:
    shamt_bits = 6;
    if (!((funct3 == FUNCT3_SLLI && upper_imm == 0) ||
          (funct3 == FUNCT3_SHIFT_RIGHT_OPP &&
           (upper_imm == IMM_SRLI || upper_imm == IMM_SRAI)))) {

      abort();
    }
    break;
  case OPCODE_OP_IMM_32:
    shamt_bits = 5;
    if (!((funct3 == (FUNCT3_SLLIW & 7) && upper_imm == (IMM_SLLIW >> 3)) ||
          (funct3 == FUNCT3_SHIFT_RIGHT_OPP_32 &&
           (upper_imm == IMM_SRLIW || upper_imm == IMM_SRAIW)))) {
      abort();
    }
    break;
  default:
    abort();
  }

  assert((shamt < (UINT32_C(1) << shamt_bits)) && "shift amount out of range");
  uint32_t immediate = (upper_imm << shamt_bits) | shamt;
  return create_i_type(immediate, rs1_index, funct3, rd_index, opcode);
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

uint32_t create_u_type(uint32_t upper_immediate, uint32_t rd_index, uint32_t opcode) {
  assert_valid_register(rd_index);
  assert_valid_opcode(opcode);
  upper_immediate = upper_immediate << 12;
  rd_index = rd_index << 7;
  return (uint32_t) upper_immediate | rd_index | opcode;
}

int32_t create_j_type(uint32_t immediate, uint32_t rd_index, uint32_t opcode) {
  assert_valid_register(rd_index);
  assert_valid_opcode(opcode);
  // immediate must be a valid
  assert((immediate % 2 == 0) && "immediate must be a multiple of 2");
  // Map offset bits [20|10:1|11|19:12] to instruction bits [31:12].
  uint32_t imm_20 =     (immediate  & (UINT32_C(1) << 20))  << 11;
  uint32_t imm_19_12 =  immediate   & UINT32_C(0x000ff000);
  uint32_t imm_11 =     (immediate  & (UINT32_C(1) << 11))  << 9;
  uint32_t imm_10_1 =   (immediate  & UINT32_C(0x000007fe)) << 20;
  return imm_20 | imm_10_1 | imm_11 | imm_19_12 | (rd_index << 7) | opcode;
}

uint32_t create_b_type(uint32_t immediate,
                       uint32_t rs2_index,
                       uint32_t rs1_index,
                       uint32_t funct3,
                       uint32_t opcode) {
  assert_valid_register(rs2_index);
  assert_valid_register(rs1_index);
  assert_valid_funct3(funct3);
  assert_valid_opcode(opcode);
  assert(((immediate % 2) == 0) && "Immediate must be factor of 2 to align with 16 bit and 32 bit instruction");

  // Immediate is expected to 12-1 NOT 12-0
  immediate = (immediate >> 1) | (1U << 31);

  // extract bits then move them into place
  uint32_t imm_12 = ((0b1 << 11) & immediate) << 20;
  uint32_t imm_11 = ((0b1 << 10) & immediate) >> 3;
  uint32_t imm_10_5 = ((0b111111 << 4) & immediate) << 21;
  uint32_t imm_4_1 = (0b1111 & immediate) << 8;

  rs2_index = rs2_index << 20;
  rs1_index = rs1_index << 15;

  funct3 = funct3 << 12;
  opcode = opcode;

  immediate = (imm_12 | imm_11 | imm_10_5 | imm_4_1);

  return (imm_12 | imm_11 | imm_10_5 | imm_4_1) | rs2_index | rs1_index | funct3 | opcode;
}
