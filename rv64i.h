#ifndef RV64I_H
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h> // for size_t
#define RV64I_H

enum {NUM_REGISTERS = 32};
enum {MEMORY_WORDS = 16};
enum {MEMORY_HALF_WORDS = MEMORY_WORDS * 2};
enum {MEMORY_BYTES = MEMORY_WORDS * 4};


struct machine_state {
  uint8_t memory[MEMORY_BYTES];
  uint64_t registers[NUM_REGISTERS];
  uint64_t program_counter;
};


void run_instruction(uint32_t instruction,
                     struct machine_state* machine_state);


enum status_code {
  RV64I_OK = 0,
  RV64I_ERROR_ILLEGAL_INSTRUCTION = 1,
};


enum major_opcodes: uint32_t {
  OPCODE_LOAD=       0b0000011,
  OPCODE_LOAD_FP=    0b0000111,
  OPCODE_CUSTOM_0=   0b0001011,
  OPCODE_MISC_MEM=   0b0001111,
  OPCODE_OP_IMM=     0b0010011,
  OPCODE_AUIPC=      0b0010111,
  OPCODE_OP_IMM_32=  0b0011011,

  OPCODE_STORE=      0b0100011,
  OPCODE_STORE_FP=   0b0100111,
  OPCODE_CUSTOM_1=   0b0101011,
  OPCODE_AMO=        0b0101111,
  OPCODE_OP=         0b0110011,
  OPCODE_LUI=        0b0110111,
  OPCODE_OP_32=      0b0111011,

  OPCODE_MADD=       0b1000011,
  OPCODE_MSUB=       0b1000111,
  OPCODE_NMSUB=      0b1001011,
  OPCODE_NMADD=      0b1001111,
  OPCODE_OP_FP=      0b1010011,
  OPCODE_OP_V=       0b1010111,
  OPCODE_CUSTOM_2=   0b1011011,

  OPCODE_BRANCH=     0b1100011,
  OPCODE_JALR=       0b1100111,
  OPCODE_RESERVED=   0b1101011,
  OPCODE_JAL=        0b1101111,
  OPCODE_SYSTEM=     0b1110011,
  OPCODE_OP_VE=      0b1110111,
  OPCODE_CUSTOM_3=   0b1111011
};


enum funct3_enums: uint32_t {
  FUNCT3_ADDW_SUBW=          0b000,
  FUNCT3_SHIFT_RIGHT=        0b101,
  FUNCT3_SLLW=               0b001,
  FUNCT3_LB=         0b000,
  FUNCT3_LH=         0b001,
  FUNCT3_LW=         0b010,
  FUNCT3_LBU=        0b100,
  FUNCT3_LHU=        0b101,

  // RV64I stuff
  FUNCT3_LWU=        0b110,
  FUNCT3_LD=         0b011,
  FUNCT3_ADDI=         0b000,
  FUNCT3_SLTI=         0b010,
  FUNCT3_SLTIU=        0b011,
  FUNCT3_XORI=         0b100,
  FUNCT3_ORI=          0b110,
  FUNCT3_ANDI=         0b111,
  FUNCT3_SLLI=         0b001,

  FUNCT3_SHIFT_RIGHT_OPP=   0b101,

  // OP_IMMEDIATE_32
  FUNCT3_ADDIW=                0b000,
  FUNCT3_SLLIW=                0b001,
  SLLIW_IMM=                    0b000,
  FUNCT3_SHIFT_RIGHT_OPP_32=    0b101,


  // store_funct3_enums
  FUNCT3_SB=         0b000,
  FUNCT3_SH=         0b001,
  FUNCT3_SW=         0b010,
  FUNCT3_SD=         0b011,

  // branch_funct3_enums
  FUNCT3_BEQ=        0b000,
  FUNCT3_BNE=        0b001,
  FUNCT3_BLT=        0b100,
  FUNCT3_BGE=        0b101,
  FUNCT3_BLTU=       0b110,
  FUNCT3_BGEU=       0b111,

  // JALR_funct3_enum
  JALR_OPCODE = 000,
};

enum funct7_enusm: uint32_t {
  FUNCT7_ADDW=               0b0000000,
  FUNCT7_SUBW=               0b0100000,
  FUNCT7_SLLW=               0b0000000,
  FUNCT7_SRLW=               0b0000000,
  FUNCT7_SRAW=               0b0100000,
};

enum immediate_id_enums: uint32_t {
  /* enum op_imm_funct3_enums: uint32_t { */
  IMM_SLLI=     0b000000,
  IMM_SRLI=     0b000000,
  IMM_SRAI=     0b010000,

  /* enum op_imm_32_enums: uint32_t { */
  IMM_SRLIW=    0b0000000,
  IMM_SRAIW=    0b0100000,

  SLLIW=        0b0000000001, // we combine funct7 (technically has len ) with funct3
};

enum MISC_MEM_enum: uint32_t {
  FENCE =     0b000,
  FENCE_TSO = 0b1000001100110000000000000, // includes all FENCE_TSO fields. best way to guarantee ID
  PAUSE =     0b0000000100000000000000000,
};

enum SYSTEM_enum: uint32_t {
  ECALL =  0b0000000000000000000000000,  // includes all FENCE_TSO fields. best way to guarantee ID
  EBREAK = 0b0000000000010000000000000,  // includes all FENCE_TSO fields. best way to guarantee ID
};

// backing numbers are funct7 + funct3
// sub_funct7 = 0100000; sub_func3 = 000
enum op_enums: uint32_t {
  ADD=         0b0000000000,
  SUB=         0b0100000000,
  SLL=         0b0000000001,
  SLT=         0b0000000010,
  SLTU=        0b0000000011,
  XOR=         0b0000000100,
  SRL=         0b0000000101,
  SRA=         0b0100000101,
  OR=          0b0000000110,
  AND=         0b0000000111,
};


void print_registers(const uint64_t registers[NUM_REGISTERS]);
void print_memory(const uint8_t memory[]);
void print_bits_impl(uint64_t value, unsigned width);
#define print_bits(x) print_bits_impl((uint64_t)(x), sizeof(x) * 8)

uint32_t create_i_type(uint32_t immediate, int32_t rs1, int32_t funct3, int32_t rd, int32_t opcode);
uint32_t create_i_type_shamt(uint32_t shamt, int32_t rs1_index, int32_t rd_index,
                             int32_t opcode, int32_t funct3, uint32_t upper_imm);
uint32_t create_s_type(uint32_t immediate, int32_t rs2_index, int32_t rs1_index, int32_t funct3, int32_t opcode);
uint32_t create_r_type(uint32_t funct7_funct3, int32_t rs2_index, int32_t rs1_index, int32_t rd_index, int32_t opcode);
uint32_t create_u_type(uint32_t upper_immediate, uint32_t rd_index, uint32_t opcode);
uint32_t create_j_type(uint32_t immediate, uint32_t rd_index, uint32_t opcode);
uint32_t create_b_type(uint32_t immediate, uint32_t rs2_index, uint32_t rs1_index, uint32_t funct3, uint32_t opcode);
uint32_t bits(uint32_t instruction, int start, int end);
uint64_t bits64(uint64_t bits, int start, int end);

// EXTRACT DATA FROM INSTRUCTION
int get_opcode(uint32_t instruction);
uint32_t get_rs1_index(uint32_t instruction);
uint32_t get_rs2_index(uint32_t instruction);
uint32_t get_rd_index(uint32_t instruction);
uint32_t get_funct3(uint32_t instruction);
uint32_t get_funct7(uint32_t instruction);
int32_t get_i_immediate(uint32_t instruction);
int32_t get_s_immediate(uint32_t instruction);
int32_t get_j_immediate(uint32_t instruction);
uint32_t get_b_immediate(uint32_t instruction);
int32_t get_upper_immediate(uint32_t instruction);

// GET PARTICULAR OPCODES
int opcode_bits_1_0(uint32_t instruction);
int opcode_bits_4_2(uint32_t instruction);
int opcode_bits_6_5(uint32_t instruction);

// GET VALUE STACK POINTER
uint64_t stack_pointer(const uint64_t registers[NUM_REGISTERS]);
uint64_t return_address(const uint64_t registers[NUM_REGISTERS]);
uint64_t alternate_return_address(const uint64_t registers[NUM_REGISTERS]);

// WRITE DATA FROM MEMORY GIVEN AN ADDRESS
uint64_t get_double_word(uint8_t const memory[], uint64_t address);
uint32_t get_word(uint8_t const memory[], uint64_t address);
uint16_t get_half_word(uint8_t const memory[], uint64_t address);
uint8_t get_byte(uint8_t const memory[], uint64_t address);

// WRITE DATA TO MEMORY GIVEN AN ADDRESS AND VALUE
void write_byte(uint8_t memory[], uint64_t address, uint64_t value);
void write_half_word(uint8_t memory[], uint64_t address, uint64_t value);
void write_word(uint8_t memory[], uint64_t address, uint64_t value);
void write_double_word(uint8_t memory[], uint64_t address, uint64_t value);

uint32_t sign_extend_32(uint32_t value, int length);
uint64_t sign_extend_64(uint64_t field, int width);

#endif
