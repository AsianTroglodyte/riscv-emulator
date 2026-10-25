#include "unity/src/unity.h"
#include "rv64i.h"
#include <inttypes.h>
#include <stddef.h>
#include <assert.h>

void setUp(void) {}

void tearDown(void) {}

struct load_test_case {
  int32_t  immediate;
  uint64_t rs1_val;
  uint64_t memory_value;
  uint64_t expected;
};

void test_load_instruction(const struct load_test_case cases[],
                           size_t case_count,
                           uint32_t funct3,
                           int width_in_bytes) {

  for (size_t i = 0; i < case_count; ++i) {
    uint64_t memory_address  = (uint64_t) (int64_t) cases[i].immediate + cases[i].rs1_val;
    struct machine_state state = {0};

    // Fill surrounding bytes with nonzero values so incorrectly wider loads affect the result
    // | Byte    | `0x80` | `0x00` | `0x00` | `0x00` |` - (incorrectly) loading 2 or 4 bytes results in 0x80 anyway
    // | Byte    | `0x80` | `0x12` | `0x34` | `0x56` |` - (incorrectly) loading 2 or 4 bytes results in different vals
    for (size_t byte_index = 0; byte_index < sizeof(state.memory); ++byte_index) {
      state.memory[byte_index] = 10;
    }

    if (width_in_bytes == 1) {
      write_byte(state.memory, memory_address, cases[i].memory_value);
    } else if (width_in_bytes == 2) {
      write_half_word(state.memory, memory_address, cases[i].memory_value);
    } else if (width_in_bytes == 4) {
      write_word(state.memory, memory_address, cases[i].memory_value);
    } else if (width_in_bytes == 8) {
      write_double_word(state.memory, memory_address, cases[i].memory_value);
    }

    state.registers[10] = cases[i].rs1_val;
    run_instruction(create_i_type(cases[i].immediate, 10, funct3, 11, LOAD), &state);
    TEST_ASSERT_EQUAL_UINT64(cases[i].expected, state.registers[11]);
  }
}

void test_LW(void) {
  struct load_test_case cases[] = {
    {.immediate = 1, .rs1_val = 3, .memory_value = 1, .expected = 1},
    {.immediate = 2, .rs1_val = 10,.memory_value = 1020343, .expected = 1020343},
    {.immediate = 2, .rs1_val = 10,.memory_value = -4, .expected = UINT64_MAX - 3}, // check if -1 is sign extended;
    {.immediate = 2, .rs1_val = 10, .memory_value = 0x80000000, .expected = 0xFFFFFFFF80000000}, // check for sign extension
    {.immediate = -2048, .rs1_val = 2052,.memory_value = 4, .expected = 4}, // immediate negative boundary
    {.immediate = 2047, .rs1_val = -2043,.memory_value = 4, .expected = 4}, // immediate positive boundary
    {.immediate = -1, .rs1_val = 13,.memory_value = 10, .expected = 10}, // negative immediate;
    {.immediate = 13, .rs1_val = -1,.memory_value = 10, .expected = 10}, // negative rs1_val;
  };
  test_load_instruction(cases, sizeof(cases)/sizeof(cases[0]), LW, 4);

  struct machine_state state = {0};
  state.registers[30] = 10;
  uint32_t instruction = create_i_type(4, 0, LW, 30, LOAD); // instruction should x0 = 10 to x0 = 0;
  run_instruction(instruction, &state);
  TEST_ASSERT_EQUAL_UINT64(0, state.registers[30]);
}

void test_LB(void) {
  struct load_test_case cases[] = {
    {.immediate = 1, .rs1_val = 3, .memory_value = 1, .expected = 1},
    {.immediate = 1, .rs1_val = 3,.memory_value = 123, .expected = 123},
    {.immediate = 1, .rs1_val = 3,.memory_value = -4, .expected = UINT64_MAX - 3}, // check if -1 is sign extended;
    {.immediate = 1, .rs1_val = 3, .memory_value = 0x80, .expected = 0xFFFFFFFFFFFFFF80}, // check for sign extension
    {.immediate = -2048, .rs1_val = 2052,.memory_value = 4, .expected = 4}, // immediate negative boundary
    {.immediate = 2047, .rs1_val = -2043,.memory_value = 4, .expected = 4}, // immediate positive boundary
    {.immediate = -1, .rs1_val = 13,.memory_value = 10, .expected = 10}, // negative immediate;
    {.immediate = 13, .rs1_val = -1,.memory_value = 10, .expected = 10}, // negative rs1_val;
  };
  test_load_instruction(cases, sizeof(cases)/sizeof(cases[0]), LB, 1);

  struct machine_state state1 = {0};
  state1.registers[30] = 10;
  uint32_t instruction = create_i_type(4, 0, LB, 30, LOAD); // instruction should x0 = 10 to x0 = 0;
  run_instruction(instruction, &state1);
  TEST_ASSERT_EQUAL_UINT64(0, state1.registers[30]);
}

void test_LH(void) {
  struct load_test_case cases[] = {
    {.immediate = 1, .rs1_val = 3, .memory_value = 1, .expected = 1},
    {.immediate = 1, .rs1_val = 3,.memory_value = 123, .expected = 123},
    {.immediate = 1, .rs1_val = 3,.memory_value = -4, .expected = UINT64_MAX - 3}, // check if -1 is sign extended;
    {.immediate = 1, .rs1_val = 3, .memory_value = 0x8000, .expected = 0xFFFFFFFFFFFF8000}, // check for sign extension
    {.immediate = -2048, .rs1_val = 2052,.memory_value = 4, .expected = 4}, // immediate negative boundary
    {.immediate = 2047, .rs1_val = -2043,.memory_value = 4, .expected = 4}, // immediate positive boundary
    {.immediate = -1, .rs1_val = 13,.memory_value = 10, .expected = 10}, // negative immediate;
    {.immediate = 13, .rs1_val = -1,.memory_value = 10, .expected = 10}, // negative rs1_val;
  };
  test_load_instruction(cases, sizeof(cases)/sizeof(cases[0]), LH, 2);

  struct machine_state state1 = {0};
  state1.registers[30] = 10;
  uint32_t instruction = create_i_type(4, 0, LH, 30, LOAD); // instruction should x30 = 10 to x30 = 0;
  run_instruction(instruction, &state1);
  TEST_ASSERT_EQUAL_UINT64(0, state1.registers[30]);
}

void test_LD() {
  struct load_test_case cases[] = {
    {.immediate = 1, .rs1_val = 7, .memory_value = 1, .expected = 1},
    {.immediate = 1, .rs1_val = 7,.memory_value = 123, .expected = 123},
    {.immediate = -2048, .rs1_val = 2052,.memory_value = 4, .expected = 4}, // immediate negative boundary
    {.immediate = 1, .rs1_val = 7,.memory_value = 0x8000000000000000, .expected = 0x8000000000000000},
    {.immediate = 1, .rs1_val = 7,.memory_value = 0x0123456789ABCDEF, .expected = 0x0123456789ABCDEF},
    {.immediate = 1, .rs1_val = 7,.memory_value = UINT64_MAX, .expected = UINT64_MAX},
    {.immediate = 2047, .rs1_val = -2043,.memory_value = 4, .expected = 4}, // immediate positive boundary
    {.immediate = -1, .rs1_val = 9,.memory_value = 10, .expected = 10}, // negative immediate;
    {.immediate = 9, .rs1_val = -1,.memory_value = 10, .expected = 10}, // negative rs1_val;
  };
  test_load_instruction(cases, sizeof(cases)/sizeof(cases[0]), LD, 8);

  struct machine_state state1 = {0};
  state1.registers[30] = 10;
  uint32_t instruction = create_i_type(4, 0, LD, 30, LOAD); // instruction should x30 = 10 to x30 = 0;
  run_instruction(instruction, &state1);
  TEST_ASSERT_EQUAL_UINT64(0, state1.registers[30]);
}

void test_LBU(void) {
  struct load_test_case cases[] = {
    {.immediate = 1, .rs1_val = 3, .memory_value = 1, .expected = 1},
    {.immediate = 1, .rs1_val = 3,.memory_value = 123, .expected = 123},
    {.immediate = 1, .rs1_val = 3,.memory_value = -4, .expected = (uint64_t) 0xFC}, // check for zero extension;
    {.immediate = 1, .rs1_val = 3, .memory_value = 0x80, .expected = 0x0000000000000080}, // check for zero extension
    {.immediate = -2048, .rs1_val = 2052,.memory_value = 4, .expected = 4}, // immediate negative boundary
    {.immediate = 2047, .rs1_val = -2043,.memory_value = 4, .expected = 4}, // immediate positive boundary
    {.immediate = -1, .rs1_val = 13,.memory_value = 10, .expected = 10}, // negative immediate;
    {.immediate = 13, .rs1_val = -1,.memory_value = 10, .expected = 10}, // negative rs1_val;
  };

  test_load_instruction(cases, sizeof(cases)/sizeof(cases[0]), LBU, 1);

  struct machine_state state1 = {0};
  state1.registers[30] = 10;
  uint32_t instruction = create_i_type(4, 0, LBU, 30, LOAD); // instruction should x30 = 10 to x30 = 0;
  run_instruction(instruction, &state1);
  TEST_ASSERT_EQUAL_UINT64(0, state1.registers[30]);
}

void test_LHU(void) {
    struct load_test_case cases[] = {
    {.immediate = 1, .rs1_val = 3, .memory_value = 1, .expected = 1},
    {.immediate = 1, .rs1_val = 3,.memory_value = 123, .expected = 123},
    {.immediate = 1, .rs1_val = 3,.memory_value = -4, .expected = (uint64_t) 0xFFFC}, // check for zero extension;
    {.immediate = 1, .rs1_val = 3, .memory_value = 0x8000, .expected = 0x0000000000008000}, // check for zero extension
    {.immediate = -2048, .rs1_val = 2052,.memory_value = 4, .expected = 4}, // immediate negative boundary
    {.immediate = 2047, .rs1_val = -2043,.memory_value = 4, .expected = 4}, // immediate positive boundary
    {.immediate = -1, .rs1_val = 13,.memory_value = 10, .expected = 10}, // negative immediate;
    {.immediate = 13, .rs1_val = -1,.memory_value = 10, .expected = 10}, // negative rs1_val;
  };
  test_load_instruction(cases, sizeof(cases)/sizeof(cases[0]), LHU, 2);

  struct machine_state state1 = {0};
  state1.registers[30] = 10;
  uint32_t instruction = create_i_type(4, 0, LHU, 30, LOAD); // instruction should x30 = 10 to x30 = 0;
  run_instruction(instruction, &state1);
  TEST_ASSERT_EQUAL_UINT64(0, state1.registers[30]);
}

void test_LWU() {
  struct load_test_case cases[] = {
    {.immediate = 1, .rs1_val = 3, .memory_value = 1, .expected = 1},
    {.immediate = 1, .rs1_val = 3,.memory_value = 123, .expected = 123},
    {.immediate = 1, .rs1_val = 3,.memory_value = -4, .expected = (uint64_t) 0xFFFFFFFC}, // check for zero extension;
    {.immediate = 1, .rs1_val = 3, .memory_value = 0x80, .expected = 0x0000000000000080}, // check for zero extension
    {.immediate = -2048, .rs1_val = 2052,.memory_value = 4, .expected = 4}, // immediate negative boundary
    {.immediate = 2047, .rs1_val = -2043,.memory_value = 4, .expected = 4}, // immediate positive boundary
    {.immediate = -1, .rs1_val = 13,.memory_value = 10, .expected = 10}, // negative immediate;
    {.immediate = 13, .rs1_val = -1,.memory_value = 10, .expected = 10}, // negative rs1_val;
  };
  test_load_instruction(cases, sizeof(cases)/sizeof(cases[0]), LWU, 4);

  struct machine_state state1 = {0};
  state1.registers[30] = 10;
  uint32_t instruction = create_i_type(4, 0, LWU, 30, LOAD); // instruction should x30 = 10 to x30 = 0;
  run_instruction(instruction, &state1);
  TEST_ASSERT_EQUAL_UINT64(0, state1.registers[30]);
}

struct store_test_case {
  int32_t immediate;
  uint64_t rs2;
  uint64_t rs1;
  uint64_t expected;
};

void test_store_instruction(const struct store_test_case cases[],
                            size_t case_count,
                            uint32_t funct3,
                            size_t width_in_bytes) {
  for (size_t i = 0; i < case_count;+ ++i) {
    /* assert((cases[i].immediate >= -2048|| cases[i].immediate <= 2047 ) && "immediate must not be larger 12 bits can represent"); */

    struct machine_state state = {0};
    // integer wraparound properly handles negative immediates anyway.
    uint64_t address = (uint64_t) cases[i].immediate + cases[i].rs1;
    state.registers[10] = cases[i].rs2;
    state.registers[11] = cases[i].rs1;

    for (size_t byte_index = 0; byte_index < sizeof(state.memory); ++byte_index) {
      state.memory[byte_index] = 10;
    }

    uint32_t instruction = create_s_type(cases[i].immediate, 10, 11, funct3, STORE);
    run_instruction(instruction, &state);

    for (size_t offset = 0; offset < width_in_bytes; ++offset) {
      uint8_t expected_byte = (uint8_t) (cases[i].expected >> (8 * offset));
      TEST_ASSERT_EQUAL_UINT8(expected_byte, state.memory[address + offset]);
    }
  }
}

void test_SB(void) {
  const struct store_test_case cases[] = {
    {.immediate = 10, .rs2 = 0, .rs1 = 6, .expected = 0},
    {.immediate = 10, .rs2 = 10, .rs1 = 6, .expected = 10},
    {.immediate = 10, .rs2 = -10, .rs1 = 6, .expected = -10},
    {.immediate = -10, .rs2 = 10, .rs1 = 26, .expected = 10},
    {.immediate = -2048, .rs2 = 10, .rs1 = 2048 + 16, .expected = 10},
    {.immediate = 2047, .rs2 = 10, .rs1 = -2047 + 16, .expected = 10},
    {.immediate = 10, .rs2 = UINT8_MAX, .rs1 = 6, .expected = UINT8_MAX},
    {.immediate = 10, .rs2 = -1, .rs1 = 6, .expected = UINT8_MAX},
  };

  test_store_instruction(cases, sizeof(cases)/sizeof(cases[0]), SB, 1);
}

void test_SH(void) {
  const struct store_test_case cases[] = {
    {.immediate = 10, .rs2 = 0, .rs1 = 6, .expected = 0},
    {.immediate = 10, .rs2 = 10, .rs1 = 6, .expected = 10},
    {.immediate = 10, .rs2 = -10, .rs1 = 6, .expected = -10},
    {.immediate = -10, .rs2 = 10, .rs1 = 16 + 10, .expected = 10},
    {.immediate = -2048, .rs2 = 10, .rs1 = 2048 + 16, .expected = 10},
    {.immediate = 2047, .rs2 = 10, .rs1 = -2047 + 16, .expected = 10},
    {.immediate = 10, .rs2 = UINT16_MAX, .rs1 = 6, .expected = UINT16_MAX},
    {.immediate = 10, .rs2 = -1, .rs1 = 6, .expected = UINT16_MAX},
  };

  test_store_instruction(cases, sizeof(cases)/sizeof(cases[0]), SH, 2);
}

void test_SW(void) {
  const struct store_test_case cases[] = {
    {.immediate = 10, .rs2 = 0, .rs1 = 6, .expected = 0},
    {.immediate = 10, .rs2 = 10, .rs1 = 6, .expected = 10},
    {.immediate = 10, .rs2 = -10, .rs1 = 6, .expected = -10},
    {.immediate = -10, .rs2 = 10, .rs1 = 16 + 10, .expected = 10},
    {.immediate = -2048, .rs2 = 10, .rs1 = 2048 + 16, .expected = 10},
    {.immediate = 2047, .rs2 = 10, .rs1 = -2047 + 16, .expected = 10},
    {.immediate = 10, .rs2 = UINT32_MAX, .rs1 = 6, .expected = UINT32_MAX},
    {.immediate = 10, .rs2 = -1, .rs1 = 6, .expected = UINT32_MAX},
  };

  test_store_instruction(cases, sizeof(cases)/sizeof(cases[0]), SW, 4);
}

void test_SD(void) {
  const struct store_test_case cases[] = {
    {.immediate = 10, .rs2 = 0, .rs1 = 6, .expected = 0},
    {.immediate = 10, .rs2 = 10, .rs1 = 6, .expected = 10},
    {.immediate = 10, .rs2 = -10, .rs1 = 6, .expected = -10},
    {.immediate = -10, .rs2 = 10, .rs1 = 16 + 10, .expected = 10},
    {.immediate = -2048, .rs2 = 10, .rs1 = 2048 + 16, .expected = 10},
    {.immediate = 2047, .rs2 = 10, .rs1 = -2047 + 16, .expected = 10},
    {.immediate = 10, .rs2 = UINT64_MAX, .rs1 = 6, .expected = UINT64_MAX},
    {.immediate = 10, .rs2 = -1, .rs1 = 6, .expected = UINT64_MAX},
  };

  test_store_instruction(cases, sizeof(cases)/sizeof(cases[0]), SD, 8);
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
    struct machine_state state = {0};
    state.registers[rs1_index] = cases[i].rs1_value;
    uint32_t instruction = create_i_type(cases[i].immediate,
                                         rs1_index,
                                         funct3,
                                         rd_index,
                                         opcode);
    run_instruction(instruction, &state);
    TEST_ASSERT_EQUAL_UINT64(cases[i].expected, state.registers[rd_index]);
  }
}

void test_addi(void) {
  const struct i_instruction_case cases[] = {
      {.rs1_value = 10, .immediate =  10, .expected =  20},             // Positive immediate
      {.rs1_value = 10, .immediate =  -10, .expected =  0},             // Negative immediate cancels rs1
      {.rs1_value = 10, .immediate =  -100, .expected =  (uint64_t)-90}, // Result wraps to a negative 64-bit value
  };

  test_i_instruction(cases, sizeof(cases) / sizeof(cases[0]), ADDI, OP_IMM);
}

void test_addiw(void) {
  const struct i_instruction_case cases[] = {
      {.rs1_value = 10, .immediate =  10, .expected =  20},             // Positive immediate
      {.rs1_value = 10, .immediate =  -10, .expected =  0},             // Negative immediate cancels rs1
      {.rs1_value = 10, .immediate =  -100, .expected =  (uint64_t)-90}, // Result wraps to a negative 64-bit value
  };

  test_i_instruction(cases, sizeof(cases) / sizeof(cases[0]), ADDI, OP_IMM);
}

void test_andi(void) {
  const struct i_instruction_case cases[] = {
      {.rs1_value = 0b1010, .immediate =  0b1010, .expected =  0b1010},             // Identical bit patterns
      {.rs1_value = 0b0101, .immediate =  0b1010, .expected =  0},                  // No overlapping set bits
      {.rs1_value = 0, .immediate =  -1, .expected =  0},                            // Zero AND sign-extended -1
      {.rs1_value = 0b1100010, .immediate =  0b1000010, .expected =  0b1000010},    // Keep only shared set bits
  };

  test_i_instruction(cases, sizeof(cases) / sizeof(cases[0]), ANDI, OP_IMM);
}

void test_ori(void) {
  const struct i_instruction_case cases[] = {
      {.rs1_value = 0b1010, .immediate =  0b1010, .expected =  0b1010},             // Identical bit patterns
      {.rs1_value = 0b1010, .immediate =  0b0101, .expected =  0b1111},             // Fill in the unset bits
      {.rs1_value = 0, .immediate =  -1, .expected =  UINT64_MAX},                   // Sign-extended immediate sets all bits
      {.rs1_value = 0b1100010, .immediate =  0b1001010, .expected =  0b1101010},     // Combine set bits from both operands
  };

  test_i_instruction(cases, sizeof(cases) / sizeof(cases[0]), ORI, OP_IMM);
}

void test_slti(void) {
  const struct i_instruction_case cases[] = {
      {.rs1_value = 1, .immediate =  9, .expected =  1},
      {.rs1_value = 9, .immediate =  1, .expected =  0},
      {.rs1_value = 2, .immediate =  2, .expected =  0},
      {.rs1_value = 0, .immediate =  -1, .expected =  0},
      {.rs1_value = UINT64_MAX, .immediate =  -1, .expected =  0},
      {.rs1_value = UINT64_MAX - 1, .immediate =  -1, .expected =  1},
      {.rs1_value = (uint64_t)-10, .immediate =  -9, .expected =  1},
  };

  test_i_instruction(cases, sizeof(cases) / sizeof(cases[0]), SLTI, OP_IMM);
}

void test_sltiu(void) {
  const struct i_instruction_case cases[] = {
      {.rs1_value = 1, .immediate =  9, .expected =  1},                  // 1 < 9
      {.rs1_value = 9, .immediate =  1, .expected =  0},                  // 9 is not less than 1
      {.rs1_value = 2, .immediate =  2, .expected =  0},                  // Equality is false
      {.rs1_value = 0, .immediate =  -1, .expected =  1},                 // 0 < UINT64_MAX
      {.rs1_value = UINT64_MAX, .immediate =  -1, .expected =  0},        // UINT64_MAX is not less than itself
      {.rs1_value = UINT64_MAX - 1, .immediate =  -1, .expected =  1},    // UINT64_MAX - 1 < UINT64_MAX
  };

  test_i_instruction(cases, sizeof(cases) / sizeof(cases[0]), SLTIU, OP_IMM);
}

void test_slli(void) {
  const struct i_instruction_case cases[] = {
      {.rs1_value = 9, .immediate =  1, .expected =  18},                     // basically
      {.rs1_value = 9, .immediate =  0b100000000001, .expected = 0},   // nothing; immediate bits [11:6] must be 000000.
      {.rs1_value = 3, .immediate =  6, .expected =  192},                    //  3 * 2^6 = 192
      {.rs1_value = -2, .immediate =  3, .expected =  -16},                   // -2 * 2 * 2 * 2 = 16
      {.rs1_value = 1, .immediate =  31, .expected =  INT64_C(1) << 31},      // 0 < UINT64_MAX
      {.rs1_value = 1, .immediate =  63, .expected =  0x8000000000000000}
  };

  test_i_instruction(cases, sizeof(cases) / sizeof(cases[0]), SLLI, OP_IMM);
}

void test_srli(void) {
  const struct i_instruction_case cases[] = {
      // Ordinary right shifts.
      // {value_shifted, (funct) | shift amount (shamt), result}
      {.rs1_value = 18, .immediate =  (SRLI_IMM << 6) | 1, .expected =  9},
      {.rs1_value = 192, .immediate =  (SRLI_IMM << 6) | 6, .expected =  3},

      // Logical shifts fill vacated high bits with zero, even when the
      // source's most significant bit is set.
      {.rs1_value = UINT64_C(0x8000000000000000), .immediate =  (SRLI_IMM << 5) | 1, .expected =
       UINT64_C(0x4000000000000000)},
      {.rs1_value = UINT64_MAX, .immediate =  (SRLI_IMM << 6) | 31, .expected =  UINT64_C(0x00000001FFFFFFFF)},

      // Boundary shift amounts and bits shifted entirely out.
      {.rs1_value = UINT64_C(0x8000000000000000), .immediate =  (SRLI_IMM << 6) | 63, .expected =  1},
      {.rs1_value = 1, .immediate =  (SRLI_IMM << 6) | 1, .expected =  0},
      {.rs1_value = 1, .immediate =  0, .expected =  1}
  };

  test_i_instruction(cases, sizeof(cases) / sizeof(cases[0]), SRLIorSRAI, OP_IMM);
}

void test_srai(void) {
  const struct i_instruction_case cases[] = {
    {.rs1_value = -8, .immediate =  (SRAI_IMM << 6) | 2, .expected = -2},   // ...1111 1000 >> 2 = ...1111 1110
    {.rs1_value = -15, .immediate =  (SRAI_IMM << 6) | 2, .expected = -4},  // ...1111 0001 >> 2 = ...1111 1100
    {.rs1_value = 100, .immediate =  (SRAI_IMM << 6) | 4, .expected = 6},   // ...0110 0100 >> 4 = ...0000 0110
    {.rs1_value = UINT64_MAX, .immediate =  (SRAI_IMM << 6) | 63, .expected = UINT64_MAX}, // ...1111 >> 63 = ...1111
    {.rs1_value = 100, .immediate =  (SRAI_IMM << 6) | 0, .expected = 100}, // ...0110 0100 >> 4 = ...0110 0100
    {.rs1_value = -8, .immediate =  (SRAI_IMM << 6) | 0, .expected = -8}
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
    struct machine_state state = {0};
    state.registers[rs1_index] = cases[i].rs1_value;
    state.registers[rs2_index] = cases[i].rs2_value;
    uint32_t instruction = create_r_type(funct_7_3,
                                         rs2_index,
                                         rs1_index,
                                         rd_index,
                                         opcode);
    run_instruction(instruction, &state);
    TEST_ASSERT_EQUAL_UINT64(cases[i].expected, state.registers[rd_index]);
  }
}


void test_ADD(void) {
  const struct r_instruction_case cases[] = {
    // int64_t rs1_value, int64_t rs2_value, uint64_t expected;
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

  test_r_instruction(cases, sizeof(cases) /sizeof(cases[0]), ADD, OP);
}

void test_SUB(void) {
  const struct r_instruction_case cases[] = {
    // int64_t rs1_value, int64_t rs2_value, uint64_t expected;
    {10, 4, 6},   // 10 + 4 = 14
    {-10, 4, -14},  // -10 + 4 = -6
    {-3, -4, 1},
    {0, 4, -4},
    {0, 0, 0},
    {UINT64_MAX, -1, 0}, // ...1111 + 1 = ...0000
    /* {INT64_MIN, 1, (INT64_C(1) << 63) - 1}, */
    {UINT64_MAX, 1, UINT64_MAX - 1}
  };

  test_r_instruction(cases, sizeof(cases) /sizeof(cases[0]), SUB, OP);
}

void test_SLL(void) {
  const struct r_instruction_case cases[] = {
    // int64_t rs1_value, int64_t rs2_value, uint64_t expected;
    {9, 1, 18},
    {3, 6, 192},                    //  3 * 2^6 = 192
    {-2, 3, -16},                // -2 * 2 * 2 * 2 = 16
    {1, 31, INT64_C(1) << 31},   // 0 < UINT64_MAX
  };

  test_r_instruction(cases, sizeof(cases) /sizeof(cases[0]), SLL, OP);
}

void test_SLT(void) {
  const struct r_instruction_case cases[] = {
    // int64_t rs1_value, int64_t rs2_value, uint64_t expected;
      {1, 9, 1},
      {9, 1, 0},
      {2, 2, 0},
      {0, -1, 0},
      {UINT64_MAX, -1, 0},
      {UINT64_MAX - 1, -1, 1},
      {(uint64_t)-10, -9, 1},
  };

  test_r_instruction(cases, sizeof(cases) /sizeof(cases[0]), SLT, OP);
}

void test_SLTU(void) {
  const struct r_instruction_case cases[] = {
    // int64_t rs1_value, int64_t rs2_value, uint64_t expected;
      {1, 9, 1},                  // 1 < 9
      {9, 1, 0},                  // 9 is not less than 1
      {2, 2, 0},                  // Equality is false
      {0, -1, 1},                 // 0 < UINT64_MAX
      {UINT64_MAX, -1, 0},        // UINT64_MAX is not less than itself
      {UINT64_MAX - 1, -1, 1},    // UINT64_MAX - 1 < UINT64_MAX
  };

  test_r_instruction(cases, sizeof(cases) /sizeof(cases[0]), SLTU, OP);
}

void test_XOR(void) {
  const struct r_instruction_case cases[] = {
    // int64_t rs1_value, int64_t rs2_value, uint64_t expected;
    {0b0101, 0b1010, 0b1111},
    {0b0101, 0b0101, 0b0000},
    {0b1100, 0b1000, 0b0100},
    {0, 0, 0}, // zero identity
    // the following aren't the best but whatever
    {UINT64_MAX, 0, UINT64_MAX},
    {UINT64_C(1) << 63, 0, UINT64_C(1) << 63},
    {0, -1, UINT64_MAX},
    {(UINT64_C(1) << 63) | 1, 1, (UINT64_C(1) << 63)}
  };

  test_r_instruction(cases, sizeof(cases) /sizeof(cases[0]), XOR, OP);
}

void test_SRL(void) {
  const struct r_instruction_case cases[] = {
    // int64_t rs1_value, int64_t rs2_value, uint64_t expected;
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

  test_r_instruction(cases, sizeof(cases) /sizeof(cases[0]), SRL, OP);
}

void test_SRA(void) {
  const struct r_instruction_case cases[] = {
    // int64_t rs1_value, int64_t rs2_value, uint64_t expected;
    {-8, (SRAI_IMM << 6) | 2, -2},   // ...1111 1000 >> 2 = ...1111 1110
    {-15, (SRAI_IMM << 6) | 2, -4},  // ...1111 0001 >> 2 = ...1111 1100
    {100, (SRAI_IMM << 6) | 4, 6},   // ...0110 0100 >> 4 = ...0000 0110
    {UINT64_MAX, (SRAI_IMM << 6) | 63, UINT64_MAX}, // ...1111 >> 63 = ...1111
    {100, (SRAI_IMM << 6) | 0, 100}, // ...0110 0100 >> 4 = ...0110 0100
  };

  test_r_instruction(cases, sizeof(cases) /sizeof(cases[0]), SRA, OP);
}

void test_OR(void) {
  const struct r_instruction_case cases[] = {
    // int64_t rs1_value, int64_t rs2_value, uint64_t expected;
      {0b1010, 0b1010, 0b1010},             // Identical bit patterns
      {0b1010, 0b0101, 0b1111},             // Fill in the unset bits
      {0, -1, UINT64_MAX},                   // Sign-extended immediate sets all bits
      {0b1100010, 0b1001010, 0b1101010},     // Combine set bits from both operands
  };

  test_r_instruction(cases, sizeof(cases) /sizeof(cases[0]), OR, OP);
}

void test_AND(void) {
  const struct r_instruction_case cases[] = {
    // int64_t rs1_value, int64_t rs2_value, uint64_t expected;
      {0b1010, 0b1010, 0b1010},             // Identical bit patterns
      {0b0101, 0b1010, 0},                  // No overlapping set bits
      {0, -1, 0},                            // Zero AND sign-extended -1
      {0b1100010, 0b1000010, 0b1000010},    // Keep only shared set bits
  };

  test_r_instruction(cases, sizeof(cases) /sizeof(cases[0]), AND, OP);
}

void test_LUI(void) {
  struct machine_state state = {0};

  // imm[31:12] = 1, so the result is 0x0000000000001000.
  state.registers[5] = UINT64_MAX;  // Confirm LUI overwrites rd.
  run_instruction(create_u_type(1, 5, LUI), &state);
  TEST_ASSERT_EQUAL_UINT64(UINT64_C(0x0000000000001000), state.registers[5]);

  // Positive value with several bits set.
  run_instruction(create_u_type(0xABCDE, 5, LUI), &state);
  TEST_ASSERT_EQUAL_UINT64(UINT64_C(0xFFFFFFFFABCDE000), state.registers[5]);

  /* // Bit 31 is set: the 32-bit result is sign-extended on RV64. */
  run_instruction(create_u_type(0x80000, 5, LUI), &state);
  TEST_ASSERT_EQUAL_UINT64(UINT64_C(0xFFFFFFFF80000000), state.registers[5]);

  /* // All 20 immediate bits set: sign-extended result is -4096. */
  run_instruction(create_u_type(0xFFFFF, 5, LUI), &state);
  TEST_ASSERT_EQUAL_UINT64(UINT64_C(0xFFFFFFFFFFFFF000), state.registers[5]);
}

void test_AUIPC(void) {
  struct machine_state state = {0};

  // positive immediates
  state.program_counter = 12;
  run_instruction(create_u_type(12, 2, AUIPC), &state);
  TEST_ASSERT_EQUAL_UINT64(((12 << 12) + 12) , state.registers[2]);

  // negative immediates
  state.program_counter = 12;
  run_instruction(create_u_type(-1, 2, AUIPC), &state);
  TEST_ASSERT_EQUAL_UINT64(((uint64_t)-1 << 12) + 12, state.registers[2]); // satisfy warning with (uin64_t)

  state.program_counter = 12;
  run_instruction(create_u_type(-123424, 2, AUIPC), &state);
  TEST_ASSERT_EQUAL_UINT64(((uint64_t)-123424 << 12) + 12, state.registers[2]); // satisfy warning with (uin64_t)
}

void test_JAL(void) {
  struct machine_state state = {0};

  state.program_counter = 12;
  run_instruction(create_j_type(10, 10, JAL), &state);
  TEST_ASSERT_EQUAL_UINT64(12 + 4, state.registers[10]);
  TEST_ASSERT_EQUAL_UINT64(10 + 12, state.program_counter);

  state.program_counter = 12;
  run_instruction(create_j_type(-8, 11, JAL), &state);
  TEST_ASSERT_EQUAL_UINT64(12 + 4, state.registers[11]);
  TEST_ASSERT_EQUAL_UINT64(4, state.program_counter);
}

void test_JALR(void) {
  struct machine_state state = {0};

  // POSITIVE IMMEDIATES
  state.program_counter = 12;
  state.registers[10] = 12;
  run_instruction(create_i_type(12, 10, JALR_OPCODE, 20, JALR), &state);
  TEST_ASSERT_EQUAL_UINT64(24, state.program_counter);

  // NEGATIVE IMMEDIATE
  state.program_counter = 12;
  state.registers[10] = 12;
  run_instruction(create_i_type(-4, 10, JALR_OPCODE, 20, JALR), &state);
  TEST_ASSERT_EQUAL_UINT64(8, state.program_counter);
}

struct branch_test_case {
  uint64_t rs1_value;
  uint64_t rs2_value;
  uint64_t pc;
  int32_t immediate;
  uint64_t expected_pc_val;
};

void test_branch_instructions(const struct branch_test_case cases[],
                             size_t case_count,
                             uint32_t funct3) {
  for (size_t i = 0; i < case_count; ++i) {
    struct machine_state state = {0};
    state.program_counter = cases[i].pc;
    state.registers[10] = cases[i].rs2_value;
    state.registers[11] = cases[i].rs1_value;
    run_instruction(create_b_type(cases[i].immediate, 10, 11, funct3, BRANCH), &state);
    TEST_ASSERT_EQUAL_UINT64(cases[i].expected_pc_val, state.program_counter);
  }
}

void test_BEQ(void) {
  /* struct machine_state state = {0}; */

  struct branch_test_case cases[] = {
    // rs1_value, rs2_value, pc, immediate, expected_pc_val;
    {0, 0, 12, 12, 24}, // equal; positive immediate
    {0, 0, 12, -4, 8},  // equal; negative immediate
    {0, 1, 12, 12, 16}, // noteq; branch not taken
    {-1, -1, 12, 12, 24}, // equal; negative register values; branch taken
    {-1, 1, 12, 12, 16}, // noteq; negative and positive register values; branch not taken
    {0, 0, 12, 0, 12}, // 0 displacement
    // 64 bits. get truncation errors
    {(uint64_t) 2 << 40, (uint64_t) 1 << 40, 12, 12, 16}, // noteq; branch not taken
    {(uint64_t) 1 << 40, (uint64_t) 1 << 40, 12, 12, 24}, // equal; branch taken
  };

  test_branch_instructions(cases, sizeof(cases)/sizeof(cases[0]), BEQ);
}

void test_BNE() {
  struct branch_test_case cases[] = {
    // rs1_value, rs2_value, pc, immediate, expected_pc_val;
    {0, 1, 12, 12, 24}, // noteq; positive immediate
    {0, 1, 12, -4, 8},  // noteq; negative immediate
    {0, 0, 12, 12, 16}, // equal; branch not taken
    {-1, -1, 12, 12, 16}, // equal; negative register values; not branched
    {-1, -2, 12, 12, 24}, // notequal; negative register values; branched
    {-1, 1, 12, 12, 24}, // noteq; negative and positive register values; branched
    {0, 1, 12, 0, 12}, // 0 displacement
    // 64 bits. get truncation errors
    {(uint64_t) 1 << 40, (uint64_t) 2 << 40, 12, 12, 24}, // noteq; branch taken
    {(uint64_t) 1 << 40, (uint64_t) 1 << 40, 12, 12, 16}, // equal; branch not taken
  };

  test_branch_instructions(cases, sizeof(cases)/sizeof(cases[0]), BNE);
}

void test_BLT() {
  struct branch_test_case cases[] = {
    // rs1_value, rs2_value, pc, immediate, expected_pc_val;
    {0, 1, 12, 12, 24}, // less than; positive immediate
    {0, 1, 12, -4, 8},  // less than; negative immediate
    {0, 0, 12, 12, 16}, // equal; NOT less than; not branched
    {-1, -1, 12, 12, 16}, // equal; NOT less than; negative register values; not branched
    {-1, -2, 12, 12, 16}, // NOT less than; negative register values; not branched
    {-1, 1, 12, 12, 24}, // less than; negative and positive register values; branched
    {0, 1, 12, 0, 12}, // 0 displacement
    // 64 bits. get truncation errors
    {(uint64_t) 1 << 40, (uint64_t) 2 << 40, 12, 12, 24}, // lt; branch taken
    {(uint64_t) 2 << 40, (uint64_t) 1 << 40, 12, 12, 16}, // NOT lt; branch not taken
  };

  test_branch_instructions(cases, sizeof(cases)/sizeof(cases[0]), BLT);
}

void test_BGE() {
  struct branch_test_case cases[] = {
    // rs1_value, rs2_value, pc, immediate, expected_pc_val;
    {1, 0, 12, 12, 24}, // geq; positive immediate
    {1, 0, 12, -4, 8},  // geq; negative immediate
    {0, 0, 12, 12, 24}, // geq;
    {0, 1, 12, 12, 16}, // not geq;
    {-1, -1, 12, 12, 24}, // geq; negative register values;
    {-1, -2, 12, 12, 24}, // geq; negative register values; branch taken
    {-2, -1, 12, 12, 16}, // NOT geq; negative register values; no branch
    {-1, 1, 12, 12, 16}, // NOT geq than; neg and pos register values; not
    {1, -1, 12, 12, 24}, // geq; neg and pos register values; branched
    {1, 0, 12, 0, 12}, // 0 displacement
    // 64 bits. get truncation errors
    {(uint64_t) 2 << 40, (uint64_t) 1 << 40, 12, 12, 24}, // geq; branch taken
    {(uint64_t) 1 << 40, (uint64_t) 2 << 40, 12, 12, 16}, // NOT geq; branch not taken
  };

  test_branch_instructions(cases, sizeof(cases)/sizeof(cases[0]), BGE);
}

void test_BLTU() {
  struct branch_test_case cases[] = {
    // rs1_value, rs2_value, pc, immediate, expected_pc_val;
    {0, 1, 12, 12, 24}, // ltu; positive immediate
    {0, 1, 12, -4, 8},  // ltu; negative immediate
    {0, 0, 12, 12, 16}, // not ltu
    {-1, 1, 12, 12, 16}, // NOT ltu; -1 unsigned is higher
    {1, -1, 12, 12, 24}, // ltu; -1 unsigned is higher
    {-1, -2, 12, 12, 16}, // NOT ltu; larger negs = larger unsigned
    {-2, -1, 12, 12, 24}, // ltu; larger negs = larger unsigned
    {-1, 0xFFFFFFFFFFFFFFFF, 12, 12, 16}, // equal so NOT ltu;
    {-2, 0xFFFFFFFFFFFFFFFE, 12, 12, 16}, // equal so NOT ltu;
    {-2, 0xFFFFFFFFFFFFFFFF, 12, 12, 24}, // Unequal; unsigned less-than; taken
    {0, 1, 12, 0, 12}, // 0 displacement
    // 64 bits. get truncation errors
    {(uint64_t) 1 << 40, (uint64_t) 2 << 40, 12, 12, 24}, // lt; branch taken
    {(uint64_t) 2 << 40, (uint64_t) 1 << 40, 12, 12, 16}, // NOT lt; branch not taken
  };

  test_branch_instructions(cases, sizeof(cases)/sizeof(cases[0]), BLTU);
}

void test_BGEU() {
  struct branch_test_case cases[] = {
    // rs1_value, rs2_value, pc, immediate, expected_pc_val;
    {1, 0, 12, 12, 24}, // geq; positive immediate
    {1, 0, 12, -4, 8},  // geq; negative immediate
    {0, 0, 12, 12, 24}, // geq
    {-1, 1, 12, 12, 24}, // Unsigned greater-than-or-equal; taken
    {1, -1, 12, 12, 16}, // Not unsigned greater-than-or-equal; not taken
    {-1, -2, 12, 12, 24}, // geq; larger negs = larger unsigned
    {-2, -1, 12, 12, 16}, // NOT geq; larger negs = larger unsigned
    {-1, 0xFFFFFFFFFFFFFFFF, 12, 12, 24}, // eq;
    {-2, 0xFFFFFFFFFFFFFFFE, 12, 12, 24}, // neq;
    {-2, 0xFFFFFFFFFFFFFFFF, 12, 12, 16}, // Unequal; unsigned less-than; not taken
    {1, 0, 12, 0, 12}, // 0 displacement
    // 64 bits. get truncation errors
    {(uint64_t) 2 << 40, (uint64_t) 1 << 40, 12, 12, 24}, // geq; branch taken
    {(uint64_t) 1 << 40, (uint64_t) 2 << 40, 12, 12, 16}, // NOT geq; branch not taken
  };
  /* printf("Cases: %d \n", sizeof(cases)/sizeof(cases[0])); */
  test_branch_instructions(cases, sizeof(cases)/sizeof(cases[0]), BGEU);
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

void test_u_create(void) {
  // SIMPLE
  uint32_t u_instruction_1 = create_u_type(0, 0, LUI);
  // funct7=0b0000000 rs2=01010, rs1=00000, funct3=000, rd=00001, opcode=0110011
  TEST_ASSERT_EQUAL(UINT32_C(0b0110111), u_instruction_1);

  uint32_t u_instruction_2 = create_u_type(-1, 6, LUI);
  // funct7=0b0100000 rs2=00110, rs1=00111, funct3=000, rd=00000, opcode=0110011
  TEST_ASSERT_EQUAL((UINT32_MAX << 12) | (0b110 << 7)  | LUI, u_instruction_2);
}

void test_b_create(void) {
  // TEST IMMEDIATE
  // 1 = 0b1; 11 = 0b1; 10-5 = 0b111011; 4-1 = 0b1000;
  uint32_t instruction = create_b_type(0b1111101110000, 0, 0, 0, 0);
  /* print_bits(instruction); */
  /* print_bits((0b1 << 31) | (0b1  << 7) | (0b111011 << 25) | (0b1000 << 8)); */
  // NOTE: 1st bit not encoded as immediate must be multiple of two for instruction alignment hence: 0b1000 << 8
  TEST_ASSERT_EQUAL_UINT32((0b1 << 31) | (0b1  << 7) | (0b111011 << 25) | (0b1000 << 8), instruction);

  // TEST OPCODE AND FUNCT 3
  instruction = create_b_type(0, 0, 0, BGEU, BRANCH);
  TEST_ASSERT_EQUAL_UINT32((BGEU << 12) | BRANCH, instruction);

  // TEST rs1_index AND rs2_index
  instruction = create_b_type(0, 0b10001, 0b10001, 0, 0);
  TEST_ASSERT_EQUAL_UINT32((0b10001 << 20) | (0b10001 << 15), instruction);
}


struct j_create_case {
  uint32_t imm_10_1;
  uint32_t imm_11;
  uint32_t imm_19_12;
  uint32_t imm_20;
  uint32_t rd_index;
};

void test_j_create_helper(const struct j_create_case cases[],
                          size_t case_count,
                          uint32_t opcode) {
  for (size_t i = 0; i < case_count; ++i) {
    uint32_t imm_10_1 = cases[i].imm_10_1;
    uint32_t imm_11 = cases[i].imm_11;
    uint32_t imm_19_12 = cases[i].imm_19_12;
    uint32_t imm_20 = cases[i].imm_20;
    uint32_t thing = (cases[i].rd_index << 7) | opcode;
    uint32_t immediate = (imm_20 << 19) | (imm_19_12 << 11) | (imm_11 << 10) | imm_10_1;

    uint32_t instruction = create_j_type(immediate, 10, JAL);
    TEST_ASSERT_EQUAL((imm_20 << 31) |
                      (imm_19_12 << 12) |
                      (imm_11 << 20) |
                      (imm_10_1 << 21) |
                      thing, instruction);
  }
}


void test_j_create(void) {
  const struct j_create_case cases[] = {
    // imm_10_1, imm_11, imm_19_12, imm_20, rd_index
    {0, 0, 0, 0b1, 10},
    {0, 0, 0b11111111, 0b1, 10},
    {0, 0b1, 0b11111111, 0b1, 10},
    {0b1111111111, 0b1, 0b11111111, 0b1, 10},
    {0b1010010111, 0b1, 0b10110011, 0b0, 10}
  };

  test_j_create_helper(cases, 5, JAL);
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
  RUN_TEST(test_j_create);
  RUN_TEST(test_u_create);
  RUN_TEST(test_b_create);
  RUN_TEST(test_get_s_immediate);

  // test load
  RUN_TEST(test_LW);
  RUN_TEST(test_LH);
  RUN_TEST(test_LB);
  RUN_TEST(test_LBU);
  RUN_TEST(test_LHU);
  RUN_TEST(test_LWU);
  RUN_TEST(test_LD);

  // test stores
  RUN_TEST(test_SB);
  RUN_TEST(test_SH);
  RUN_TEST(test_SW);
  RUN_TEST(test_SD);

  // test arithmetic with immediates
  RUN_TEST(test_addi);
  RUN_TEST(test_andi);
  RUN_TEST(test_ori);
  RUN_TEST(test_slti);
  RUN_TEST(test_sltiu);
  RUN_TEST(test_slli);
  RUN_TEST(test_srli);
  RUN_TEST(test_srai);

  // test arithmetic with JUST registers
  RUN_TEST(test_ADD);
  RUN_TEST(test_SUB);
  RUN_TEST(test_SLL);
  RUN_TEST(test_SLT);
  RUN_TEST(test_SLTU);
  RUN_TEST(test_XOR);
  RUN_TEST(test_SRL);
  RUN_TEST(test_SRA);
  RUN_TEST(test_OR);
  RUN_TEST(test_AND);

  // test upper immediate stuff (weird ones)
  RUN_TEST(test_LUI);
  RUN_TEST(test_AUIPC);

  // teste JUMP instructions
  RUN_TEST(test_JAL);
  RUN_TEST(test_JALR);

  // test branch instructions
  RUN_TEST(test_BEQ);
  RUN_TEST(test_BNE);
  RUN_TEST(test_BLT);
  RUN_TEST(test_BGE);
  RUN_TEST(test_BLTU);
  RUN_TEST(test_BGEU);

  UNITY_END();
}
