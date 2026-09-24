#include <stdio.h>
#include <stdint.h>
#define NUM_REGISTERS 32


void print_register_values(const uint64_t registers[NUM_REGISTERS]) {
  // add bounds check later
  
  for (int i = 0; i < NUM_REGISTERS; i++) {
    printf("register x%d = %u\n", i, registers[i]);
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

int main() {
  uint64_t registers[NUM_REGISTERS] = {0, 1, 2, 0, 0, 5};

  print_register_values(registers);

  uint64_t cur_stack_pointer = stack_pointer(registers);
  uint64_t cur_return_address = return_address(registers);
  uint64_t cur_alternate_return_address = alternate_return_address(registers);

  printf("cur_stack_pointer: %u\n", cur_stack_pointer);
  printf("cur_return_address: %u\n", cur_return_address);
  printf("cur_alternate_return_address: %u\n", cur_alternate_return_address);

  return 0;
}
