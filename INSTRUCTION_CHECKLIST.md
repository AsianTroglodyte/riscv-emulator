# Base Integer Instruction Checklist

Source review of `run_instruction` in `rv64i.c`.

**Checked means an execution handler exists, not that the instruction is fully verified or ISA-conformant.** Print-only placeholders are unchecked. Tests were not run for this checklist.

## Shared RV32I / RV64I instructions

The following handlers currently operate on the emulator's 64-bit registers. They are a starting point for RV32I, not evidence of a working RV32 execution mode.

### Upper immediate
- [x] LUI
- [x] AUIPC

### Jumps
- [x] JAL
- [x] JALR — correctness fixes needed; see notes below.

### Branches
- [x] BEQ
- [x] BNE
- [x] BLT
- [x] BGE
- [x] BLTU
- [x] BGEU

### Loads
- [x] LB
- [x] LH
- [x] LW
- [x] LBU
- [x] LHU

### Stores
- [x] SB
- [x] SH
- [x] SW

### Immediate arithmetic, comparisons, and logic
- [x] ADDI
- [x] SLTI
- [x] SLTIU
- [ ] XORI
- [x] ORI
- [x] ANDI
- [x] SLLI
- [x] SRLI
- [x] SRAI

### Register arithmetic, comparisons, and logic
- [x] ADD
- [x] SUB
- [x] SLL — shift-count masking needed.
- [x] SLT
- [x] SLTU
- [x] XOR
- [x] SRL — shift-count masking needed.
- [x] SRA — shift-count masking needed; signed shift is implementation-defined in C11.
- [x] OR
- [x] AND

### Ordering and environment
- [ ] FENCE — MISC_MEM currently only prints a message.
- [ ] ECALL — SYSTEM currently only prints a message.
- [ ] EBREAK — SYSTEM currently only prints a message.

## RV64I additions

### Loads and stores
- [x] LWU
- [x] LD
- [x] SD

### 32-bit-result immediate operations
- [x] ADDIW
- [x] SLLIW
- [ ] SRLIW
- [ ] SRAIW

### 32-bit-result register operations
- [ ] ADDW
- [ ] SUBW
- [ ] SLLW
- [ ] SRLW
- [ ] SRAW

These W operations produce a 32-bit result sign-extended to 64 bits. OP_IMM_32 and OP_32 are currently print-only placeholders.

## Cross-cutting completion work

- [ ] Add an explicit RV32 execution mode or separate RV32 implementation: 32-bit register/PC arithmetic, signed comparisons, wrapping, and five-bit shift amounts/encoding validation.
- [ ] Reject RV64-only instructions in RV32 mode.
- [ ] Enforce x0: reads return zero and writes are discarded.
- [ ] Mask register shift counts: low six bits for RV64, low five bits for RV32 (and RV64 W shifts).
- [ ] Fix JALR: calculate the target using the original source before writing rd, and clear target bit 0.
- [ ] Review full-width address calculation: memory addresses currently pass through a uint32_t variable even in the 64-bit machine.
- [ ] Make illegal/unsupported instruction handling consistent; printing and continuing is not a complete trap/error policy.
- [ ] Define memory bounds, alignment, and instruction-target alignment handling.
- [ ] Audit C integer conversions and signed overflow, including PC/address calculations.
- [ ] Add architectural regression tests, including known encoded instruction words independent of encoder constants.

## Scope notes

- NOP and other assembler pseudoinstructions do not require separate execution handlers; they expand into real instructions.
- FENCE.I belongs to Zifencei, and CSR instructions belong to Zicsr; neither is part of the current base I checklist.
- Multiplication/division, atomics, floating point, compressed instructions, vectors, and privileged instructions are separate extensions/specifications.
- A single-hart emulator may implement FENCE without a host memory barrier if its execution model already satisfies the ordering requirements, but it should still decode and deliberately handle the instruction.

## Next steps

Work in this order:

- [ ] Finish the remaining instruction handlers above.
- [ ] 1. Build tooling: apply warning flags to builds and add AddressSanitizer/UndefinedBehaviorSanitizer development builds.
- [ ] 2. Execution errors: define explicit success/fault results, consistent failure-state behavior, and caller-owned diagnostics.
- [ ] 3. RV32/RV64 semantics: deliberately handle register width, wrapping, signed comparisons, shift rules, and mode-specific instructions.
- [ ] 4. Stronger tests: add independent instruction encodings, property tests, differential testing, and decoder fuzzing.
- [ ] 5. Module boundaries: separate cohesive responsibilities with small public interfaces and private implementation details; organize tests by instruction category.
- [ ] Review and finish the cross-cutting completion checklist above.

Some roadmap steps overlap with cross-cutting items; mark those items complete as they are implemented and verified rather than repeating the work.

