#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include <gflags/gflags.h>

#include "../types.h"

namespace netlist {
#include "../netlist_6502.h"
}

extern "C" {
#include "../netlist_sim.h"
#include "../perfect6502.h"
}

#include "disasm.h"

void log(std::string str);

// Nodes that aren't in netlist_6502.h or have been commented out
namespace netlist {
enum {
  sync_ = 539,   // fetch
  clock1 = 156,  // t0
  clock2 = 1536, // t1
  t2 = 971,
  t3 = 1567,
  t4 = 690,
  t5 = 909,
  vec1 = 1481, // t6
};
} // namespace netlist

void printState(void *state) {
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

  auto a = readA(state);
  auto x = readX(state);
  auto y = readY(state);
  auto p = readP(state);

  // Print disassembly before first output of new instruction
  if (t2 == 0) {
    log(std::format("{}\n",
                    ark::disassemble(ir, memory[pc], memory[pc + 1]).c_str()));
  }

  std::string display = std::format(
      "PC:{:04X} (PC):{:02X} IR:{:02X} Sync:{} T:{}{}{}{}{}{}{} "
      "Addr:{:04X} Data:{:02X} RW:{} A:{:02X} X:{:02X} Y:{:02X} P:{:02X}\n",
      pc, pc_deref, ir, sync_, t0, t1, t2, t3, t4, t5, t6, address_bus,
      data_bus, rw, a, x, y, p);
  log(display);

  if (sync_ != 0) {
    log("\n");
  }
}

void setupMemory(std::string filename) {
  if (!filename.empty()) {
    std::ifstream input_file(filename, std::ios::binary | std::ios::ate);
    if (!input_file) {
      filename.clear();
    } else {
      std::memset(memory, 0x00, sizeof(memory));
      size_t file_bytes = std::min(static_cast<size_t>(input_file.tellg()),
                                   static_cast<size_t>(0x10000));
      input_file.seekg(0);
      input_file.read((char *)(&memory[0]), file_bytes);
      input_file.close();
      log(std::format("Loaded {} bytes from {}\n", file_bytes, filename));
      log(std::format("Reset vector: {:02X}{:02X}\n", memory[0xfffd],
                      memory[0xfffc]));
    }
  }

  if (filename.empty()) {
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
}

DEFINE_bool(interactive, false,
            "Run in interactive mode. "
            "Advance one cycle or instruction at a time "
            "until exit is requested. During operation:\n"
            "    c|t|<space>    advance one cycle\n"
            "    s|i|<enter>    advance one instruction\n"
            "    q|x|e|<esc>    quit");

DEFINE_int32(cycles, 100, "Run this many full cycles and exit.");

DEFINE_string(file, "", "Binary program file to execute");

#if WITH_CURSES
#include <ncurses.h>
#endif

void advanceHalfClock(void *state) { step(state); }

void advanceFullClock(void *state) {
  step(state);
  auto c0 = readNodes(state, 1, (nodenum_t[]){netlist::clk0});
  if (c0 != 0) {
    step(state);
  }
}

void advanceInstruction(void *state, bool print_state) {
  advanceFullClock(state);
  if (print_state) {
    printState(state);
  }
  auto t2 = readNodes(state, 1, (nodenum_t[]){netlist::t2});
  while (t2 != 0) {
    advanceFullClock(state);
    if (print_state) {
      printState(state);
    }
    t2 = readNodes(state, 1, (nodenum_t[]){netlist::t2});
  }
}

void log(std::string str) {
#if WITH_CURSES
  addstr(str.c_str());
  refresh();
#else
  printf("%s", str.c_str());
#endif
}

void init() {
#if WITH_CURSES
  initscr();
  scrollok(stdscr, true);
  cbreak();
  noecho();
  keypad(stdscr, true);
#endif
}

void shutdown() {
#if WITH_CURSES
  endwin();
#endif
}

int get_input() {
#if WITH_CURSES
  int ch = getch();
#else
  char ch = 0;
  std::cin.get(ch);
#endif
  return static_cast<int>(ch);
}

void help_interactive() {
  log("Interactive mode:\n");
  log("  s|i|<enter> Step one instruction\n");
  log("  c|t|<space> Step one full clock tick\n");
  log("  h|?         Show this help\n");
  log("  q|x|e|<esc> Quit\n");
  log("===\n\n");
}

int main(int argc, char **argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  init();

  // Initial state
  setupMemory(FLAGS_file);
  void *state = initAndResetChip();
  printState(state);
  log("===\n");

  if (!FLAGS_interactive) {
    // Non-interactive mode. Run for FLAGS_cycles cycles and exit
    for (auto i = 0; i < FLAGS_cycles; i++) {
      advanceFullClock(state);
      printState(state);
    }
  } else {
    // Interactive mode. Execute user commands.
    std::vector<int> cmd_tick_clock{'c', 't', ' '};
    std::vector<int> cmd_step_instruction{'s', 'i'};
    std::vector<int> cmd_help = {'h', '?'};
    std::vector<int> cmd_quit = {'q', 'x', 'e', 27};
#if WITH_CURSES
    cmd_step_instruction.push_back(10);
#endif

    bool go = true;
    while (go) {

      int ch = get_input();

      // Quit
      if (std::find(cmd_quit.begin(), cmd_quit.end(), ch) != cmd_quit.end()) {
        go = false;
      }

      // Tick
      else if (std::find(cmd_tick_clock.begin(), cmd_tick_clock.end(), ch) !=
               cmd_tick_clock.end()) {
        advanceFullClock(state);
        printState(state);
      }

      // Step
      else if (std::find(cmd_step_instruction.begin(),
                         cmd_step_instruction.end(),
                         ch) != cmd_step_instruction.end()) {
        advanceInstruction(state, true);
      }

      // Help
      else if (std::find(cmd_help.begin(), cmd_help.end(), ch) !=
               cmd_help.end()) {
        help_interactive();
      }

      // Unknown
      else {
        // do nothing
      }
    }
  }

  destroyChip(state);

  shutdown();
  return 0;
}
