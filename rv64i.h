#ifndef RV64I_H
#define RV64I_H
#include <stdint.h>

void run_instruction(uint32_t instruction);

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


enum {NUM_REGISTERS = 32};

#endif
