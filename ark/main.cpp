#include <iostream>

extern "C" {
#include "../perfect6502.h"
}

void printState(void *state) {
  auto pc = readPC(state);
  printf("(PC):%02X ", memory[pc]);
  chipStatus(state);
}

void setupMemory() {
  std::vector<uint8_t> program = {
      0xa9, 0x00, 0x20, 0x10, 0x00, 0x4c, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x40, 0xe8, 0x88, 0xe6, 0x0f, 0x38, 0x69, 0x02, 0x60};

  std::memcpy(&memory[0x0000], program.data(), program.size());
}

int main(int argc, char **argv) {
  setupMemory();

  void *state = initAndResetChip();
  printState(state);
  printf("===\n");

  for (auto i = 0; i < 120; i++) {
    step(state);
    printState(state);
  }

  destroyChip(state);
  return 0;
}
