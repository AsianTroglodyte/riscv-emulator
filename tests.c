#include "unity/src/unity.h"
#include "rv64i.h"
#include <inttypes.h>
#include <stddef.h>

void setUp(void) {}

void tearDown(void) {}

void test_LW(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_WORDS; i++) {
    memory[i * 4] = i * 4;
  }

  // POSITIVE NUMBERS
  uint32_t instruction = create_i_type(4, 4, LW, 10, LOAD);
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(4, registers[10]);

  // NEGATIVE IMMEDIATE
  registers[4] = 8;
  uint32_t instruction1 = create_i_type(-4, 4, LW, 10, LOAD);
  run_instruction(instruction1, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(4, registers[10]);

  // NEGATIVE NUMBERS
  registers[4] = 0;
  memory[4] = (int8_t)-4;
  memory[5] = 255;
  memory[6] = 255;
  memory[7] = 255;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(UINT64_MAX-3, registers[10]);
}

void test_LB(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};

  for (uint32_t i = 0; i < MEMORY_BYTES; i++) {
    memory[i] = i;
  }

  uint32_t instruction = create_i_type(10, 4, LB, 10, LOAD);
  // POSITIVE NUMBERS
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(10, registers[10]);

  // NEGATIVE NUMBERS
  memory[10] = (int8_t)-4;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(UINT64_MAX-3, registers[10]);

  // NEGATIVE IMMEDIATE
  registers[4] = 8;
  uint32_t instruction1 = create_i_type(-4, 4, LB, 10, LOAD);
  run_instruction(instruction1, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(4, registers[10]);
}

void test_LH(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_HALF_WORDS; i++) {
    memory[i * 2] = i * 2;
  }

  uint32_t instruction = create_i_type(10, 4, LH, 10, LOAD);
  // POSITIVE NUMBERS
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(10, registers[10]);

  // NEGATIVE NUMBERS
  memory[10] = (int8_t)-4;
  memory[11] = 255;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(UINT64_MAX-3, registers[10]);

  // NEGATIVE IMMEDIATE
  registers[4] = 8;
  uint32_t instruction1 = create_i_type(-4, 4, LH, 10, LOAD);
  run_instruction(instruction1, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(4, registers[10]);
}

void test_LBU(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_BYTES; i++) {
    memory[i] = i ;
  }

  // POSITIVE NUMBERS IN MEMORY
  uint32_t instruction = create_i_type(10, 4, LBU, 10, LOAD);
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(10, registers[10]);

  // NEGATIVE NUMBERS IN MEMORY
  memory[10] = (int8_t)-4;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(UINT8_MAX - 3, registers[10]);

  // NEGATIVE IMMEDIATE
  registers[4] = 8;
  uint32_t instruction1 = create_i_type(-4, 4, LBU, 10, LOAD);
  run_instruction(instruction1, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(4, registers[10]);
}

void test_LHU(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_HALF_WORDS; i++) {
    memory[i * 2] = i * 2;
  }

  // POSITIVE NUMBERS
  uint32_t instruction = create_i_type(10, 4, LHU, 10, LOAD);
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(10, registers[10]);

  // NEGATIVE NUMBERS
  memory[10] = (int8_t)-4;
  memory[11] = 255;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(UINT16_MAX - 3, registers[10]);

  // NEGATIVE IMMEDIATE
  registers[4] = 8;
  memory[4] = 4;
  uint32_t instruction1 = create_i_type(-4, 4, LHU, 10, LOAD);
  run_instruction(instruction1, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(4, registers[10]);
}

void test_SB(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_BYTES; i++) {
    memory[i] = i;
  }

  // TEST WITH ZERO
  registers[10] = 0;
  uint32_t instruction = create_s_type(10, 10, 4, SB, STORE);
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(0, memory[10]);

  // TEST WITH STORING NEGATIVE VALUE
  registers[10] = -10;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(UINT8_MAX - 9, get_byte(memory, 10));

  /* // TEST WITH POSITIVE */
  registers[10] = 5;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(5, get_byte(memory, 10));

  /* // TEST WITH NEGATIVE IMMEDIATE */
  registers[4] = 20;
  registers[10] = 5;
  uint32_t instruction_2 = create_s_type(-10, 10, 4, SB, STORE);
  run_instruction(instruction_2, memory, registers);
  TEST_ASSERT_EQUAL(5, get_byte(memory, 10));
}

void test_SH(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_HALF_WORDS; i++) {
    memory[i * 2] = i * 2;
  }
  // TEST WITH ZERO
  registers[10] = 0;
  uint32_t instruction = create_s_type(10, 10, 4, SH, STORE);
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(0, get_half_word(memory, 10));

  // TEST WITH STORING NEGATIVE VALUE
  registers[10] = -10;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(UINT16_MAX - 9, get_half_word(memory, 10));

  /* // TEST WITH POSITIVE */
  registers[10] = 5;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(5, get_half_word(memory, 10));

  /* // TEST WITH NEGATIVE IMMEDIATE */
  registers[4] = 20;
  registers[10] = 5;
  uint32_t instruction_2 = create_s_type(-10, 10, 4, SH, STORE);
  run_instruction(instruction_2, memory, registers);
  TEST_ASSERT_EQUAL(5, get_half_word(memory, 10));
}

void test_SW(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_WORDS; i++) {
    memory[i * 4] = i * 4;
  }

  // TEST WITH ZERO
  registers[8] = 0;
  uint32_t instruction = create_s_type(8, 10, 4, SW, STORE);
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(0, get_word(memory, 8));

  // TEST WITH STORING NEGATIVE VALUE
  registers[10] = -10;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(UINT32_MAX - 9, get_word(memory, 8));

  /* // TEST WITH POSITIVE */
  registers[10] = 5;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(5, get_word(memory, 8));

  /* // TEST WITH NEGATIVE IMMEDIATE */
  registers[4] = 16;
  registers[10] = 5;
  uint32_t instruction_2 = create_s_type(-8, 10, 4, SW, STORE);
  run_instruction(instruction_2, memory, registers);
  TEST_ASSERT_EQUAL(5, get_word(memory, 8));
}

void test_SD(void) {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};
  uint8_t memory[MEMORY_BYTES] = {};
  for (uint32_t i = 0; i < MEMORY_WORDS / 2; i++) {
    memory[i * 8] = i * 8;
  }

  // TEST WITH ZERO
  uint32_t instruction = create_s_type(8, 10, 4, SD, STORE);
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL(0, get_double_word(memory, 8));

  // TEST WITH STORING NEGATIVE VALUE
  registers[10] = -10;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(UINT64_MAX - 9, get_double_word(memory, 8));

  // TEST WITH POSITIVE
  registers[10] = 5;
  run_instruction(instruction, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(5, get_double_word(memory, 8));

  // TEST WITH NEGATIVE IMMEDIATE
  registers[4] = 16;
  registers[10] = 5;
  uint32_t instruction_2 = create_s_type(-8, 10, 4, SD, STORE);
  run_instruction(instruction_2, memory, registers);
  TEST_ASSERT_EQUAL_UINT64(5, get_double_word(memory, 8));
}

struct i_instruction_case {
  uint64_t rs1_value;
  int32_t immediate;
  uint64_t expected;
};

// Each case supplies rs1's value, the immediate, and the expected rd value.
static void test_i_instruction(const struct i_instruction_case cases[],
                               size_t case_count,
                               uint32_t funct3,
                               uint32_t opcode) {
  const uint32_t rs1_index = 1;
  const uint32_t rd_index = 10;

  for (size_t i = 0; i < case_count; ++i) {
    uint64_t registers[NUM_REGISTERS] = {0};
    uint8_t memory[MEMORY_BYTES] = {0};
    registers[rs1_index] = cases[i].rs1_value;

    uint32_t instruction = create_i_type(cases[i].immediate,
                                         rs1_index,
                                         funct3,
                                         rd_index,
                                         opcode);
    run_instruction(instruction, memory, registers);
    TEST_ASSERT_EQUAL_UINT64(cases[i].expected, registers[rd_index]);
  }
}

void test_addi(void) {
  const struct i_instruction_case cases[] = {
      {10, 10, 20},             // Positive immediate
      {10, -10, 0},             // Negative immediate cancels rs1
      {10, -100, (uint64_t)-90}, // Result wraps to a negative 64-bit value
  };

  test_i_instruction(cases, sizeof(cases) / sizeof(cases[0]), ADDI, OP_IMM);
}

void test_andi(void) {
  const struct i_instruction_case cases[] = {
      {0b1010, 0b1010, 0b1010},             // Identical bit patterns
      {0b0101, 0b1010, 0},                  // No overlapping set bits
      {0, -1, 0},                            // Zero AND sign-extended -1
      {0b1100010, 0b1000010, 0b1000010},    // Keep only shared set bits
  };

  test_i_instruction(cases, sizeof(cases) / sizeof(cases[0]), ANDI, OP_IMM);
}

void test_ori(void) {
  const struct i_instruction_case cases[] = {
      {0b1010, 0b1010, 0b1010},             // Identical bit patterns
      {0b1010, 0b0101, 0b1111},             // Fill in the unset bits
      {0, -1, UINT64_MAX},                   // Sign-extended immediate sets all bits
      {0b1100010, 0b1001010, 0b1101010},     // Combine set bits from both operands
  };

  test_i_instruction(cases, sizeof(cases) / sizeof(cases[0]), ORI, OP_IMM);
}

void test_slti(void) {
  const struct i_instruction_case cases[] = {
      {1, 9, 1},
      {9, 1, 0},
      {2, 2, 0},
      {0, -1, 0},
      {UINT64_MAX, -1, 0},
      {UINT64_MAX - 1, -1, 1},
      {(uint64_t)-10, -9, 1},
  };

  test_i_instruction(cases, sizeof(cases) / sizeof(cases[0]), SLTI, OP_IMM);
}

void test_sltiu(void) {
  const struct i_instruction_case cases[] = {
      {1, 9, 1},                  // 1 < 9
      {9, 1, 0},                  // 9 is not less than 1
      {2, 2, 0},                  // Equality is false
      {0, -1, 1},                 // 0 < UINT64_MAX
      {UINT64_MAX, -1, 0},        // UINT64_MAX is not less than itself
      {UINT64_MAX - 1, -1, 1},    // UINT64_MAX - 1 < UINT64_MAX
  };

  test_i_instruction(cases, sizeof(cases) / sizeof(cases[0]), SLTIU, OP_IMM);
}

void test_slli(void) {
  const struct i_instruction_case cases[] = {
      {9, 1, 18},                     // basically
      {3, 6, 192},                    //  3 * 2^6 = 192
      {-2, 3, -16},                   // -2 * 2 * 2 * 2 = 16
      {1, 31, INT64_C(1) << 31}, // 0 < UINT64_MAX
  };

  test_i_instruction(cases, sizeof(cases) / sizeof(cases[0]), SLLI, OP_IMM);
}

void test_srli(void) {
  const struct i_instruction_case cases[] = {
      // Ordinary right shifts.
      // {value_shifted, (funct) | shift amount (shamt), result}
      {18, (SRLI_IMM << 5) | 1, 9},
      {192, (SRLI_IMM << 5) | 6, 3},

      // Logical shifts fill vacated high bits with zero, even when the
      // source's most significant bit is set.
      {UINT64_C(0x8000000000000000), (SRLI_IMM << 5) | 1,
       UINT64_C(0x4000000000000000)},
      {UINT64_MAX, (SRLI_IMM << 5) | 31, UINT64_C(0x00000001FFFFFFFF)},

      // Boundary shift amounts and bits shifted entirely out.
      {UINT64_C(0x8000000000000000), (SRLI_IMM << 5) | 63, 1},
      {1, (SRLI_IMM << 5) | 1, 0},
  };

  test_i_instruction(cases, sizeof(cases) / sizeof(cases[0]), SRLIorSRAI, OP_IMM);
}

void test_srai(void) {
  const struct i_instruction_case cases[] = {
    {-8, (SRAI_IMM << 6) | 2, -2},   // ...1111 1000 >> 2 = ...1111 1110
    {-15, (SRAI_IMM << 6) | 2, -4},  // ...1111 0001 >> 2 = ...1111 1100
    {100, (SRAI_IMM << 6) | 4, 6},   // ...0110 0100 >> 4 = ...0000 0110
    {UINT64_MAX, (SRAI_IMM << 6) | 63, UINT64_MAX}, // ...1111 >> 63 = ...1111
    {100, (SRAI_IMM << 6) | 0, 100}, // ...0110 0100 >> 4 = ...0110 0100
  };

  test_i_instruction(cases, sizeof(cases) / sizeof(cases[0]), SRLIorSRAI, OP_IMM);
}

struct r_instruction_case {
  int64_t rs1_value;
  int64_t rs2_value;
  uint64_t expected;
};

static void test_r_instruction(const struct r_instruction_case cases[],
                               size_t case_count,
                               uint32_t funct_7_3,
                               uint32_t opcode) {
  const uint32_t rs1_index = 1;
  const uint32_t rs2_index = 2;
  const uint32_t rd_index = 10;

  for (size_t i = 0; i < case_count; ++i) {
    uint64_t registers[NUM_REGISTERS] = {0};
    uint8_t memory[MEMORY_BYTES] = {0};
    registers[rs1_index] = cases[i].rs1_value;
    registers[rs2_index] = cases[i].rs2_value;
    uint32_t instruction = create_r_type(funct_7_3,
                                         rs2_index,
                                         rs1_index,
                                         rd_index,
                                         opcode);
    run_instruction(instruction, memory, registers);
    TEST_ASSERT_EQUAL_UINT64(cases[i].expected, registers[rd_index]);
  }
}


void test_ADD(void) {
  const struct r_instruction_case cases[] = {
    {1, 1, 2},     // 1 + 1 = 2
    {10, 4, 14},   // 10 + 4 = 14
    {-10, 4, -6},  // -10 + 4 = -6
    {-3, -4, -7},
    {0, 4, 4},
    {0, 0, 0},
    {UINT64_MAX, -1, UINT64_MAX - 1}, // -10 + 4 = -6
    {INT64_MAX, 1, INT64_C(1) << 63}, // -10 + 4 = -6
    {INT64_MIN, -1, INT64_MAX}, // -10 + 4 = -6
    {UINT64_MAX, 1, 0}
  };

  /* test_r_instruction(cases, sizeof(cases) /sizeof(cases[0]), ADD, OP); */
}

void test_SUB(void) {
  const struct r_instruction_case cases[] = {
    {10, 4, 6},   // 10 + 4 = 14
    {-10, 4, -14},  // -10 + 4 = -6
    {-3, -4, 1},
    {0, 4, -4},
    {0, 0, 0},
    {UINT64_MAX, -1, 0}, // ...1111 + 1 = ...0000
    {INT64_MIN, 1, (INT64_C(1) << 63) - 1},
    {UINT64_MAX, 1, UINT64_MAX - 1}
  };

  /* test_r_instruction(cases, sizeof(cases) /sizeof(cases[0]), SUB, OP); */
}

void test_SLL(void) {
  const struct r_instruction_case cases[] = {
    {9, 1, 18},
    {3, 6, 192},                    //  3 * 2^6 = 192
    {-2, 3, -16},                // -2 * 2 * 2 * 2 = 16
    {1, 31, INT64_C(1) << 31},   // 0 < UINT64_MAX
  };

  test_r_instruction(cases, sizeof(cases) /sizeof(cases[0]), SLL, OP);
}


void test_s_create(void) {
  // EASY: 1000 is within the signed 12-bit S-immediate range.
  uint32_t s_instruction_1 = create_s_type(1000, 6, 7, SB, STORE);
  // imm[11:5]=0011111, rs2=00110, rs1=00111, funct3=000, imm[4:0]=01000
  TEST_ASSERT_EQUAL(0b00111110011000111000010000100011, s_instruction_1);

  // NEGATIVE IMMEDIATE
  uint32_t s_instruction_2 = create_s_type(-1, 6, 7, SB, STORE);
  // should be imme_11_5(1111 111)(0 0110 rs2) (rs1 0011 1)(000 funct3) (imm_0_4 1111 1)(010 0011 opcode)
  TEST_ASSERT_EQUAL(0b11111110011000111000111110100011, s_instruction_2);

  // NEGATIVE IMMEDIATE BOUNDARY
  uint32_t s_instruction_3 = create_s_type(-2048, 6, 7, SB, STORE);
  // should be imme_11_5(1000 000)(0 0110 rs2) (rs1 0011 1)(000 funct3) (imm_0_4 0000 0)(010 0011 opcode)
  TEST_ASSERT_EQUAL(0b10000000011000111000000000100011, s_instruction_3);

  // POSITIVE IMMEDIATE BOUNDARY
  uint32_t s_instruction_4 = create_s_type(2047, 6, 7, SB, STORE);
  // should be imme_11_5(0111 111)(0 0110 rs2) (rs1 0011 1)(000 funct3) (imm_0_4 1111 1)(010 0011 opcode)
  TEST_ASSERT_EQUAL(0b01111110011000111000111110100011, s_instruction_4);
}

void test_r_create(void) {
  uint32_t r_instruction_1 = create_r_type(SUB, 6, 7, 0, OP);
  // funct7=0b0100000 rs2=00110, rs1=00111, funct3=000, rd=00000, opcode=0110011
  TEST_ASSERT_EQUAL(0b01000000011000111000000000110011, r_instruction_1);

  // ADD
  uint32_t r_instruction_2 = create_r_type(ADD, 10, 0, 1, OP);
  // funct7=0b0000000 rs2=01010, rs1=00000, funct3=000, rd=00001, opcode=0110011
  TEST_ASSERT_EQUAL(0b00000000101000000000000010110011, r_instruction_2);
}


void test_get_s_immediate(void) {
  // EASY
  uint32_t s_instruction_1 = create_s_type(1000, 6, 7, SB, STORE);
  TEST_ASSERT_EQUAL(1000, get_s_immediate(s_instruction_1));

  // NEGATIVE IMMEDIATE
  uint32_t s_instruction_2 = create_s_type(-1, 6, 7, SB, STORE);
  TEST_ASSERT_EQUAL(-1, get_s_immediate(s_instruction_2));

  // NEGATIVE IMMEDIATE BOUNDARY
  uint32_t s_instruction_3 = create_s_type(-2048, 6, 7, SB, STORE);
  // should be imme_11_5(1000 000)(0 0110 rs2) (rs1 0011 1)(000 funct3) (imm_0_4 0000 0)(010 0011 opcode)
  TEST_ASSERT_EQUAL(-2048, get_s_immediate(s_instruction_3));

  // POSITIVE IMMEDIATE BOUNDARY
  uint32_t s_instruction_4 = create_s_type(2047, 6, 7, SB, STORE);
  // should be imme_11_5(0111 111)(0 0110 rs2) (rs1 0011 1)(000 funct3) (imm_0_4 1111 1)(010 0011 opcode)
  TEST_ASSERT_EQUAL(2047, get_s_immediate(s_instruction_4));
}


int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_s_create);
  RUN_TEST(test_r_create);
  RUN_TEST(test_get_s_immediate);

  RUN_TEST(test_LW);
  RUN_TEST(test_LH);
  RUN_TEST(test_LB);
  RUN_TEST(test_LBU);
  RUN_TEST(test_LHU);
  RUN_TEST(test_SB);
  RUN_TEST(test_SH);
  RUN_TEST(test_SW);
  RUN_TEST(test_SD);
  RUN_TEST(test_addi);
  RUN_TEST(test_andi);
  RUN_TEST(test_ori);
  RUN_TEST(test_slti);
  RUN_TEST(test_sltiu);
  RUN_TEST(test_slli);
  RUN_TEST(test_srli);
  RUN_TEST(test_srai);
  RUN_TEST(test_ADD);
  RUN_TEST(test_SUB);
  RUN_TEST(test_SLL);

  UNITY_END();
}
