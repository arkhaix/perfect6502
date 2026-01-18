#include <iostream>

#include "../types.h"

namespace netlist {
#include "../netlist_6502.h"
}

extern "C" {
#include "../netlist_sim.h"
#include "../perfect6502.h"
}

// Nodes that aren't in netlist_6502.h or have been commented out
namespace netlist {
enum {
  clock1 = 156,
  clock2 = 1536,
  sync_ = 539,
  t2 = 971,
  t3 = 1567,
  t4 = 690,
  t5 = 909,
  vec1 = 1481,
};
} // namespace netlist

enum class PrintCondition { Always, LowOnly };
void printState(void *state, PrintCondition cond) {

  // halfcyc:1 phi0:0 AB:00FF D:00 RnW:1 PC:00FF A:00 X:C0 Y:00 SP:C0 P:02 IR:00

  auto c0 = readNodes(state, 1, (nodenum_t[]){netlist::clk0});
  if (cond == PrintCondition::LowOnly && c0 != 0) {
    return;
  }

  auto t0 = readNodes(state, 1, (nodenum_t[]){netlist::clock1});
  auto t1 = readNodes(state, 1, (nodenum_t[]){netlist::clock2});
  auto t2 = readNodes(state, 1, (nodenum_t[]){netlist::t2});
  auto t3 = readNodes(state, 1, (nodenum_t[]){netlist::t3});
  auto t4 = readNodes(state, 1, (nodenum_t[]){netlist::t4});
  auto t5 = readNodes(state, 1, (nodenum_t[]){netlist::t5});
  auto t6 = readNodes(state, 1, (nodenum_t[]){netlist::vec1});

  auto sync_ = readNodes(state, 1, (nodenum_t[]){netlist::sync_});

  auto ir = readIR(state);

  auto address_bus = readAddressBus(state);
  auto data_bus = readDataBus(state);
  auto rw = readRW(state);

  auto pc = readPC(state);
  auto pc_deref = memory[pc];

  auto a = (uint8_t)readNodes(
      state, 8,
      (nodenum_t[]){netlist::a0, netlist::a1, netlist::a2, netlist::a3,
                    netlist::a4, netlist::a5, netlist::a6, netlist::a7});
  auto x = (uint8_t)readNodes(
      state, 8,
      (nodenum_t[]){netlist::x0, netlist::x1, netlist::x2, netlist::x3,
                    netlist::x4, netlist::x5, netlist::x6, netlist::x7});
  auto y = (uint8_t)readNodes(
      state, 8,
      (nodenum_t[]){netlist::y0, netlist::y1, netlist::y2, netlist::y3,
                    netlist::y4, netlist::y5, netlist::y6, netlist::y7});

  printf("PC:%04X (PC):%02X IR:%02X Sync:%d T:%d%d%d%d%d%d%d Addr:%04X "
         "Data:%02X RW:%d A:%02X X:%02X Y:%02X\n",
         pc, pc_deref, ir, sync_, t0, t1, t2, t3, t4, t5, t6, address_bus,
         data_bus, rw, a, x, y);

  if (sync_ != 0) {
    printf("\n");
  }
}

void setupMemory() {
  // This is the default program from visual6502.org
  /*
  0000   A9 00                LDA #$00
  0002   20 10 00   L0002     JSR L0010
  0005   4C 02 00             JMP L0002
  0008   00                   BRK
  0009   00                   BRK
  000A   00                   BRK
  000B   00                   BRK
  000C   00                   BRK
  000D   00                   BRK
  000E   00                   BRK
  000F   40                   RTI
  0010   E8         L0010     INX
  0011   88                   DEY
  0012   E6 0F                INC $0F
  0014   38                   SEC
  0015   69 02                ADC #$02
  0017   60                   RTS
  0018   00                   BRK
  0019   00                   BRK
  */

  std::vector<uint8_t> program = {
      0xa9, 0x00, 0x20, 0x10, 0x00, 0x4c, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x40, 0xe8, 0x88, 0xe6, 0x0f, 0x38, 0x69, 0x02, 0x60};

  std::memcpy(&memory[0x0000], program.data(), program.size());
}

int main(int argc, char **argv) {
  setupMemory();

  void *state = initAndResetChip();
  printState(state, PrintCondition::Always);
  printf("===\n");

  for (auto i = 0; i < 120; i++) {
    step(state);
    printState(state, PrintCondition::LowOnly);
  }

  destroyChip(state);
  return 0;
}
