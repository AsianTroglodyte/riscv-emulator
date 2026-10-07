#ifndef RV64I_H
#include <stdint.h>

#define RV64I_H

enum {NUM_REGISTERS = 32};
enum {MEMORY_WORDS = 16};
enum {MEMORY_HALF_WORDS = MEMORY_WORDS * 2};
enum {MEMORY_BYTES = MEMORY_WORDS * 4};

void run_instruction(uint32_t instruction,
                     uint8_t memory[],
                     uint64_t registers[NUM_REGISTERS]);


enum status_code {
  RV64I_OK = 0,
  RV64I_ERROR_ILLEGAL_INSTRUCTION = 1
};


enum major_opcodes: uint32_t {
  LOAD=       0b0000011,
  LOAD_FP=    0b0000111,
  CUSTOM_0=   0b0001011,
  MISC_MEM=   0b0001111,
  OP_IMM=     0b0010011,
  AUIPC=      0b0010111,
  OP_IMM_32=  0b0011011,

  STORE=      0b0100011,
  STORE_FP=   0b0100111,
  CUSTOM_1=   0b0101011,
  AMO=        0b0101111,
  OP=         0b0110011,
  LUI=        0b0110111,
  OP_32=      0b0111011,

  MADD=       0b1000011,
  MSUB=       0b1000111,
  NMSUB=      0b1001011,
  NMADD=      0b1001111,
  OP_FP=      0b1010011,
  OP_V=       0b1010111,
  CUSTOM_2=   0b1011011,

  BRANCH=     0b1100011,
  JALR=       0b1100111,
  RESERVED=   0b1101011,
  JAL=        0b1101111,
  SYSTEM=     0b1110011,
  OP_VE=      0b1110111,
  CUSTOM_3=   0b1111011
};

enum load_funct3_enums: uint32_t {
  LB=         0b000,
  LH=         0b001,
  LW=         0b010,
  LBU=        0b100,
  LHU=        0b101
};

enum op_imm_enums: uint32_t {
  ADDI=         0b000,
  SLTI=         0b010,
  SLTIU=        0b011,
  XORI=         0b100,
  ORI=          0b110,
  ANDI=         0b111
};

enum store_funct3_enums: uint32_t {
  SB=         0b000,
  SH=         0b001,
  SW=         0b010,
  SD=         0b011
};

enum branch_funct3_enums: uint32_t {
  BEQ=        0b000,
  BNE=        0b001,
  BLT=        0b100,
  BGE=        0b101,
  BLTU=       0b110,
  BGEU=       0b111
};


void print_registers(const uint64_t registers[NUM_REGISTERS]);
void print_memory(const uint8_t memory[]);
void print_bits(const unsigned int num);
void print_8_bits(const uint8_t num);

uint32_t create_i_type(uint32_t immediate,
                       uint32_t rs1,
                       uint32_t funct3,
                       uint32_t rd,
                       uint32_t opcode);
uint32_t create_s_type(uint32_t immediate,
                       uint32_t rs1,
                       uint32_t rs2,
                       uint32_t funct3,
                       uint32_t opcode);

// EXTRACT DATA FROM INSTRUCTION
int get_opcode(uint32_t instruction);
uint32_t get_rs1_index(uint32_t instruction);
uint32_t get_rs2_index(uint32_t instruction);
uint32_t get_rd_index(uint32_t instruction);
uint32_t get_funct3(uint32_t instruction);
uint32_t get_funct7(uint32_t instruction);
int32_t get_i_immediate(uint32_t instruction);
int32_t get_s_immediate(uint32_t instruction);

// GET PARTICULAR OPCODES
int opcode_bits_1_0(uint32_t instruction);
int opcode_bits_4_2(uint32_t instruction);
int opcode_bits_6_5(uint32_t instruction);

// GET VALUE STACK POINTER
uint64_t stack_pointer(const uint64_t registers[NUM_REGISTERS]);
uint64_t return_address(const uint64_t registers[NUM_REGISTERS]);
uint64_t alternate_return_address(const uint64_t registers[NUM_REGISTERS]);

// WRITE DATA FROM MEMORY GIVEN AN ADDRESS
uint64_t get_double_word(uint8_t const memory[], uint32_t address);
uint32_t get_word(uint8_t const memory[], uint32_t address);
uint16_t get_half_word(uint8_t const memory[], uint32_t address);
uint8_t get_byte(uint8_t const memory[], uint32_t address);

// WRITE DATA TO MEMORY GIVEN AN ADDRESS AND VALUE
void write_byte(uint8_t memory[], uint32_t address, uint64_t value);
void write_double_word(uint8_t memory[], uint32_t address, uint64_t value);
void write_word(uint8_t memory[], uint32_t address, uint64_t value);
void write_half_word(uint8_t memory[], uint32_t address, uint64_t value);

int32_t sign_extend_32(uint32_t value, int length);

#endif
