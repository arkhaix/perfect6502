#include "disasm.h"

#include <cstdint>
#include <format>
#include <map>
#include <mutex>
#include <string>
#include <vector>

namespace ark {

OpcodeInfo illegal_opcode{.opcode = 0,
                          .mnemonic = Mnemonic::ILLEGAL,
                          .address_mode = AddressMode::ILLEGAL,
                          .bytes = 0};
std::vector<OpcodeInfo> opcodes(0x100, illegal_opcode);

void disasm_initialize();
std::map<Mnemonic, std::string> mnemonic_to_string;

std::once_flag disasm_init;

std::string disassemble(uint8_t opcode, uint8_t operand1, uint8_t operand2) {
  std::call_once(disasm_init, disasm_initialize);

  std::string mne = "UNK";
  auto it = mnemonic_to_string.find(opcodes[opcode].mnemonic);
  if (it != mnemonic_to_string.end()) {
    mne = it->second;
  }

  // todo: address mode handling
  std::string op1 = std::format("{:02X}", operand1);
  std::string op2 = std::format("{:02X}", operand2);

  if (opcodes[opcode].bytes == 1) {
    return mne;
  }

  if (opcodes[opcode].bytes == 2) {
    return std::format("{} {}", mne, op1);
  }

  return std::format("{} {} {}", mne, op1, op2);
}

void disasm_initialize() {
  mnemonic_to_string[Mnemonic::ILLEGAL] = "ILLEGAL";
  mnemonic_to_string[Mnemonic::ADC] = "ADC";
  mnemonic_to_string[Mnemonic::AND] = "AND";
  mnemonic_to_string[Mnemonic::ASL] = "ASL";
  mnemonic_to_string[Mnemonic::BCC] = "BCC";
  mnemonic_to_string[Mnemonic::BCS] = "BCS";
  mnemonic_to_string[Mnemonic::BEQ] = "BEQ";
  mnemonic_to_string[Mnemonic::BIT] = "BIT";
  mnemonic_to_string[Mnemonic::BMI] = "BMI";
  mnemonic_to_string[Mnemonic::BNE] = "BNE";
  mnemonic_to_string[Mnemonic::BPL] = "BPL";
  mnemonic_to_string[Mnemonic::BRK] = "BRK";
  mnemonic_to_string[Mnemonic::BVC] = "BVC";
  mnemonic_to_string[Mnemonic::BVS] = "BVS";
  mnemonic_to_string[Mnemonic::CLC] = "CLC";
  mnemonic_to_string[Mnemonic::CLD] = "CLD";
  mnemonic_to_string[Mnemonic::CLI] = "CLI";
  mnemonic_to_string[Mnemonic::CLV] = "CLV";
  mnemonic_to_string[Mnemonic::CMP] = "CMP";
  mnemonic_to_string[Mnemonic::CPX] = "CPX";
  mnemonic_to_string[Mnemonic::CPY] = "CPY";
  mnemonic_to_string[Mnemonic::DEC] = "DEC";
  mnemonic_to_string[Mnemonic::DEX] = "DEX";
  mnemonic_to_string[Mnemonic::DEY] = "DEY";
  mnemonic_to_string[Mnemonic::EOR] = "EOR";
  mnemonic_to_string[Mnemonic::INC] = "INC";
  mnemonic_to_string[Mnemonic::INX] = "INX";
  mnemonic_to_string[Mnemonic::INY] = "INY";
  mnemonic_to_string[Mnemonic::JMP] = "JMP";
  mnemonic_to_string[Mnemonic::JSR] = "JSR";
  mnemonic_to_string[Mnemonic::LDA] = "LDA";
  mnemonic_to_string[Mnemonic::LDX] = "LDX";
  mnemonic_to_string[Mnemonic::LDY] = "LDY";
  mnemonic_to_string[Mnemonic::LSR] = "LSR";
  mnemonic_to_string[Mnemonic::NOP] = "NOP";
  mnemonic_to_string[Mnemonic::ORA] = "ORA";
  mnemonic_to_string[Mnemonic::PHA] = "PHA";
  mnemonic_to_string[Mnemonic::PHP] = "PHP";
  mnemonic_to_string[Mnemonic::PLA] = "PLA";
  mnemonic_to_string[Mnemonic::PLP] = "PLP";
  mnemonic_to_string[Mnemonic::ROL] = "ROL";
  mnemonic_to_string[Mnemonic::ROR] = "ROR";
  mnemonic_to_string[Mnemonic::RTI] = "RTI";
  mnemonic_to_string[Mnemonic::RTS] = "RTS";
  mnemonic_to_string[Mnemonic::SBC] = "SBC";
  mnemonic_to_string[Mnemonic::SEC] = "SEC";
  mnemonic_to_string[Mnemonic::SED] = "SED";
  mnemonic_to_string[Mnemonic::SEI] = "SEI";
  mnemonic_to_string[Mnemonic::STA] = "STA";
  mnemonic_to_string[Mnemonic::STX] = "STX";
  mnemonic_to_string[Mnemonic::STY] = "STY";
  mnemonic_to_string[Mnemonic::TAX] = "TAX";
  mnemonic_to_string[Mnemonic::TAY] = "TAY";
  mnemonic_to_string[Mnemonic::TSX] = "TSX";
  mnemonic_to_string[Mnemonic::TXA] = "TXA";
  mnemonic_to_string[Mnemonic::TXS] = "TXS";
  mnemonic_to_string[Mnemonic::TYA] = "TYA";
  opcodes[0x69] = {.opcode = 0x69,
                   .mnemonic = Mnemonic::ADC,
                   .address_mode = AddressMode::IMM,
                   .bytes = 2};
  opcodes[0x65] = {.opcode = 0x65,
                   .mnemonic = Mnemonic::ADC,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0x75] = {.opcode = 0x75,
                   .mnemonic = Mnemonic::ADC,
                   .address_mode = AddressMode::ZPX,
                   .bytes = 2};
  opcodes[0x6d] = {.opcode = 0x6d,
                   .mnemonic = Mnemonic::ADC,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0x7d] = {.opcode = 0x7d,
                   .mnemonic = Mnemonic::ADC,
                   .address_mode = AddressMode::ABSX,
                   .bytes = 3};
  opcodes[0x79] = {.opcode = 0x79,
                   .mnemonic = Mnemonic::ADC,
                   .address_mode = AddressMode::ABSY,
                   .bytes = 3};
  opcodes[0x61] = {.opcode = 0x61,
                   .mnemonic = Mnemonic::ADC,
                   .address_mode = AddressMode::INDX,
                   .bytes = 2};
  opcodes[0x71] = {.opcode = 0x71,
                   .mnemonic = Mnemonic::ADC,
                   .address_mode = AddressMode::INDY,
                   .bytes = 2};
  opcodes[0x29] = {.opcode = 0x29,
                   .mnemonic = Mnemonic::AND,
                   .address_mode = AddressMode::IMM,
                   .bytes = 2};
  opcodes[0x25] = {.opcode = 0x25,
                   .mnemonic = Mnemonic::AND,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0x35] = {.opcode = 0x35,
                   .mnemonic = Mnemonic::AND,
                   .address_mode = AddressMode::ZPX,
                   .bytes = 2};
  opcodes[0x2d] = {.opcode = 0x2d,
                   .mnemonic = Mnemonic::AND,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0x3d] = {.opcode = 0x3d,
                   .mnemonic = Mnemonic::AND,
                   .address_mode = AddressMode::ABSX,
                   .bytes = 3};
  opcodes[0x39] = {.opcode = 0x39,
                   .mnemonic = Mnemonic::AND,
                   .address_mode = AddressMode::ABSY,
                   .bytes = 3};
  opcodes[0x21] = {.opcode = 0x21,
                   .mnemonic = Mnemonic::AND,
                   .address_mode = AddressMode::INDX,
                   .bytes = 2};
  opcodes[0x31] = {.opcode = 0x31,
                   .mnemonic = Mnemonic::AND,
                   .address_mode = AddressMode::INDY,
                   .bytes = 2};
  opcodes[0x0a] = {.opcode = 0x0a,
                   .mnemonic = Mnemonic::ASL,
                   .address_mode = AddressMode::ACC,
                   .bytes = 1};
  opcodes[0x06] = {.opcode = 0x06,
                   .mnemonic = Mnemonic::ASL,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0x16] = {.opcode = 0x16,
                   .mnemonic = Mnemonic::ASL,
                   .address_mode = AddressMode::ZPX,
                   .bytes = 2};
  opcodes[0x0e] = {.opcode = 0x0e,
                   .mnemonic = Mnemonic::ASL,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0x1e] = {.opcode = 0x1e,
                   .mnemonic = Mnemonic::ASL,
                   .address_mode = AddressMode::ABSX,
                   .bytes = 3};
  opcodes[0x90] = {.opcode = 0x90,
                   .mnemonic = Mnemonic::BCC,
                   .address_mode = AddressMode::REL,
                   .bytes = 2};
  opcodes[0xB0] = {.opcode = 0xB0,
                   .mnemonic = Mnemonic::BCS,
                   .address_mode = AddressMode::REL,
                   .bytes = 2};
  opcodes[0xF0] = {.opcode = 0xF0,
                   .mnemonic = Mnemonic::BEQ,
                   .address_mode = AddressMode::REL,
                   .bytes = 2};
  opcodes[0x30] = {.opcode = 0x30,
                   .mnemonic = Mnemonic::BMI,
                   .address_mode = AddressMode::REL,
                   .bytes = 2};
  opcodes[0xD0] = {.opcode = 0xD0,
                   .mnemonic = Mnemonic::BNE,
                   .address_mode = AddressMode::REL,
                   .bytes = 2};
  opcodes[0x10] = {.opcode = 0x10,
                   .mnemonic = Mnemonic::BPL,
                   .address_mode = AddressMode::REL,
                   .bytes = 2};
  opcodes[0x50] = {.opcode = 0x50,
                   .mnemonic = Mnemonic::BVC,
                   .address_mode = AddressMode::REL,
                   .bytes = 2};
  opcodes[0x70] = {.opcode = 0x70,
                   .mnemonic = Mnemonic::BVS,
                   .address_mode = AddressMode::REL,
                   .bytes = 2};
  opcodes[0x24] = {.opcode = 0x24,
                   .mnemonic = Mnemonic::BIT,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0x2c] = {.opcode = 0x2c,
                   .mnemonic = Mnemonic::BIT,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0x00] = {.opcode = 0x00,
                   .mnemonic = Mnemonic::BRK,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0x18] = {.opcode = 0x18,
                   .mnemonic = Mnemonic::CLC,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0xd8] = {.opcode = 0xd8,
                   .mnemonic = Mnemonic::CLD,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0x58] = {.opcode = 0x58,
                   .mnemonic = Mnemonic::CLI,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0xb8] = {.opcode = 0xb8,
                   .mnemonic = Mnemonic::CLV,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0xea] = {.opcode = 0xea,
                   .mnemonic = Mnemonic::NOP,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0x48] = {.opcode = 0x48,
                   .mnemonic = Mnemonic::PHA,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0x68] = {.opcode = 0x68,
                   .mnemonic = Mnemonic::PLA,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0x08] = {.opcode = 0x08,
                   .mnemonic = Mnemonic::PHP,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0x28] = {.opcode = 0x28,
                   .mnemonic = Mnemonic::PLP,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0x40] = {.opcode = 0x40,
                   .mnemonic = Mnemonic::RTI,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0x60] = {.opcode = 0x60,
                   .mnemonic = Mnemonic::RTS,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0x38] = {.opcode = 0x38,
                   .mnemonic = Mnemonic::SEC,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0xf8] = {.opcode = 0xf8,
                   .mnemonic = Mnemonic::SED,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0x78] = {.opcode = 0x78,
                   .mnemonic = Mnemonic::SEI,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0xaa] = {.opcode = 0xaa,
                   .mnemonic = Mnemonic::TAX,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0x8a] = {.opcode = 0x8a,
                   .mnemonic = Mnemonic::TXA,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0xa8] = {.opcode = 0xa8,
                   .mnemonic = Mnemonic::TAY,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0x98] = {.opcode = 0x98,
                   .mnemonic = Mnemonic::TYA,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0xba] = {.opcode = 0xba,
                   .mnemonic = Mnemonic::TSX,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0x9a] = {.opcode = 0x9a,
                   .mnemonic = Mnemonic::TXS,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0xc9] = {.opcode = 0xc9,
                   .mnemonic = Mnemonic::CMP,
                   .address_mode = AddressMode::IMM,
                   .bytes = 2};
  opcodes[0xc5] = {.opcode = 0xc5,
                   .mnemonic = Mnemonic::CMP,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0xd5] = {.opcode = 0xd5,
                   .mnemonic = Mnemonic::CMP,
                   .address_mode = AddressMode::ZPX,
                   .bytes = 2};
  opcodes[0xcd] = {.opcode = 0xcd,
                   .mnemonic = Mnemonic::CMP,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0xdd] = {.opcode = 0xdd,
                   .mnemonic = Mnemonic::CMP,
                   .address_mode = AddressMode::ABSX,
                   .bytes = 3};
  opcodes[0xd9] = {.opcode = 0xd9,
                   .mnemonic = Mnemonic::CMP,
                   .address_mode = AddressMode::ABSY,
                   .bytes = 3};
  opcodes[0xc1] = {.opcode = 0xc1,
                   .mnemonic = Mnemonic::CMP,
                   .address_mode = AddressMode::INDX,
                   .bytes = 2};
  opcodes[0xd1] = {.opcode = 0xd1,
                   .mnemonic = Mnemonic::CMP,
                   .address_mode = AddressMode::INDY,
                   .bytes = 2};
  opcodes[0xe0] = {.opcode = 0xe0,
                   .mnemonic = Mnemonic::CPX,
                   .address_mode = AddressMode::IMM,
                   .bytes = 2};
  opcodes[0xe4] = {.opcode = 0xe4,
                   .mnemonic = Mnemonic::CPX,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0xec] = {.opcode = 0xec,
                   .mnemonic = Mnemonic::CPX,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0xc0] = {.opcode = 0xc0,
                   .mnemonic = Mnemonic::CPY,
                   .address_mode = AddressMode::IMM,
                   .bytes = 2};
  opcodes[0xc4] = {.opcode = 0xc4,
                   .mnemonic = Mnemonic::CPY,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0xcc] = {.opcode = 0xcc,
                   .mnemonic = Mnemonic::CPY,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0xc6] = {.opcode = 0xc6,
                   .mnemonic = Mnemonic::DEC,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0xd6] = {.opcode = 0xd6,
                   .mnemonic = Mnemonic::DEC,
                   .address_mode = AddressMode::ZPX,
                   .bytes = 2};
  opcodes[0xce] = {.opcode = 0xce,
                   .mnemonic = Mnemonic::DEC,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0xde] = {.opcode = 0xde,
                   .mnemonic = Mnemonic::DEC,
                   .address_mode = AddressMode::ABSX,
                   .bytes = 3};
  opcodes[0xca] = {.opcode = 0xca,
                   .mnemonic = Mnemonic::DEX,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0x88] = {.opcode = 0x88,
                   .mnemonic = Mnemonic::DEY,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0xe8] = {.opcode = 0xe8,
                   .mnemonic = Mnemonic::INX,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0xc8] = {.opcode = 0xc8,
                   .mnemonic = Mnemonic::INY,
                   .address_mode = AddressMode::IMP,
                   .bytes = 1};
  opcodes[0x49] = {.opcode = 0x49,
                   .mnemonic = Mnemonic::EOR,
                   .address_mode = AddressMode::IMM,
                   .bytes = 2};
  opcodes[0x45] = {.opcode = 0x45,
                   .mnemonic = Mnemonic::EOR,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0x55] = {.opcode = 0x55,
                   .mnemonic = Mnemonic::EOR,
                   .address_mode = AddressMode::ZPX,
                   .bytes = 2};
  opcodes[0x4d] = {.opcode = 0x4d,
                   .mnemonic = Mnemonic::EOR,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0x5d] = {.opcode = 0x5d,
                   .mnemonic = Mnemonic::EOR,
                   .address_mode = AddressMode::ABSX,
                   .bytes = 3};
  opcodes[0x59] = {.opcode = 0x59,
                   .mnemonic = Mnemonic::EOR,
                   .address_mode = AddressMode::ABSY,
                   .bytes = 3};
  opcodes[0x41] = {.opcode = 0x41,
                   .mnemonic = Mnemonic::EOR,
                   .address_mode = AddressMode::INDX,
                   .bytes = 2};
  opcodes[0x51] = {.opcode = 0x51,
                   .mnemonic = Mnemonic::EOR,
                   .address_mode = AddressMode::INDY,
                   .bytes = 2};
  opcodes[0xe6] = {.opcode = 0xe6,
                   .mnemonic = Mnemonic::INC,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0xf6] = {.opcode = 0xf6,
                   .mnemonic = Mnemonic::INC,
                   .address_mode = AddressMode::ZPX,
                   .bytes = 2};
  opcodes[0xee] = {.opcode = 0xee,
                   .mnemonic = Mnemonic::INC,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0xfe] = {.opcode = 0xfe,
                   .mnemonic = Mnemonic::INC,
                   .address_mode = AddressMode::ABSX,
                   .bytes = 3};
  opcodes[0x4c] = {.opcode = 0x4c,
                   .mnemonic = Mnemonic::JMP,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0x6c] = {.opcode = 0x6c,
                   .mnemonic = Mnemonic::JMP,
                   .address_mode = AddressMode::IND,
                   .bytes = 3};
  opcodes[0x20] = {.opcode = 0x20,
                   .mnemonic = Mnemonic::JSR,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0xa9] = {.opcode = 0xa9,
                   .mnemonic = Mnemonic::LDA,
                   .address_mode = AddressMode::IMM,
                   .bytes = 2};
  opcodes[0xa5] = {.opcode = 0xa5,
                   .mnemonic = Mnemonic::LDA,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0xb5] = {.opcode = 0xb5,
                   .mnemonic = Mnemonic::LDA,
                   .address_mode = AddressMode::ZPX,
                   .bytes = 2};
  opcodes[0xad] = {.opcode = 0xad,
                   .mnemonic = Mnemonic::LDA,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0xbd] = {.opcode = 0xbd,
                   .mnemonic = Mnemonic::LDA,
                   .address_mode = AddressMode::ABSX,
                   .bytes = 3};
  opcodes[0xb9] = {.opcode = 0xb9,
                   .mnemonic = Mnemonic::LDA,
                   .address_mode = AddressMode::ABSY,
                   .bytes = 3};
  opcodes[0xa1] = {.opcode = 0xa1,
                   .mnemonic = Mnemonic::LDA,
                   .address_mode = AddressMode::INDX,
                   .bytes = 2};
  opcodes[0xb1] = {.opcode = 0xb1,
                   .mnemonic = Mnemonic::LDA,
                   .address_mode = AddressMode::INDY,
                   .bytes = 2};
  opcodes[0xa2] = {.opcode = 0xa2,
                   .mnemonic = Mnemonic::LDX,
                   .address_mode = AddressMode::IMM,
                   .bytes = 2};
  opcodes[0xa6] = {.opcode = 0xa6,
                   .mnemonic = Mnemonic::LDX,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0xb6] = {.opcode = 0xb6,
                   .mnemonic = Mnemonic::LDX,
                   .address_mode = AddressMode::ZPY,
                   .bytes = 2};
  opcodes[0xae] = {.opcode = 0xae,
                   .mnemonic = Mnemonic::LDX,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0xbe] = {.opcode = 0xbe,
                   .mnemonic = Mnemonic::LDX,
                   .address_mode = AddressMode::ABSY,
                   .bytes = 3};
  opcodes[0xa0] = {.opcode = 0xa0,
                   .mnemonic = Mnemonic::LDY,
                   .address_mode = AddressMode::IMM,
                   .bytes = 2};
  opcodes[0xa4] = {.opcode = 0xa4,
                   .mnemonic = Mnemonic::LDY,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0xb4] = {.opcode = 0xb4,
                   .mnemonic = Mnemonic::LDY,
                   .address_mode = AddressMode::ZPX,
                   .bytes = 2};
  opcodes[0xac] = {.opcode = 0xac,
                   .mnemonic = Mnemonic::LDY,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0xbc] = {.opcode = 0xbc,
                   .mnemonic = Mnemonic::LDY,
                   .address_mode = AddressMode::ABSX,
                   .bytes = 3};
  opcodes[0x4a] = {.opcode = 0x4a,
                   .mnemonic = Mnemonic::LSR,
                   .address_mode = AddressMode::ACC,
                   .bytes = 1};
  opcodes[0x46] = {.opcode = 0x46,
                   .mnemonic = Mnemonic::LSR,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0x56] = {.opcode = 0x56,
                   .mnemonic = Mnemonic::LSR,
                   .address_mode = AddressMode::ZPX,
                   .bytes = 2};
  opcodes[0x4e] = {.opcode = 0x4e,
                   .mnemonic = Mnemonic::LSR,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0x5e] = {.opcode = 0x5e,
                   .mnemonic = Mnemonic::LSR,
                   .address_mode = AddressMode::ABSX,
                   .bytes = 3};
  opcodes[0x09] = {.opcode = 0x09,
                   .mnemonic = Mnemonic::ORA,
                   .address_mode = AddressMode::IMM,
                   .bytes = 2};
  opcodes[0x05] = {.opcode = 0x05,
                   .mnemonic = Mnemonic::ORA,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0x15] = {.opcode = 0x15,
                   .mnemonic = Mnemonic::ORA,
                   .address_mode = AddressMode::ZPX,
                   .bytes = 2};
  opcodes[0x0d] = {.opcode = 0x0d,
                   .mnemonic = Mnemonic::ORA,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0x1d] = {.opcode = 0x1d,
                   .mnemonic = Mnemonic::ORA,
                   .address_mode = AddressMode::ABSX,
                   .bytes = 3};
  opcodes[0x19] = {.opcode = 0x19,
                   .mnemonic = Mnemonic::ORA,
                   .address_mode = AddressMode::ABSY,
                   .bytes = 3};
  opcodes[0x01] = {.opcode = 0x01,
                   .mnemonic = Mnemonic::ORA,
                   .address_mode = AddressMode::INDX,
                   .bytes = 2};
  opcodes[0x11] = {.opcode = 0x11,
                   .mnemonic = Mnemonic::ORA,
                   .address_mode = AddressMode::INDY,
                   .bytes = 2};
  opcodes[0x2a] = {.opcode = 0x2a,
                   .mnemonic = Mnemonic::ROL,
                   .address_mode = AddressMode::ACC,
                   .bytes = 1};
  opcodes[0x26] = {.opcode = 0x26,
                   .mnemonic = Mnemonic::ROL,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0x36] = {.opcode = 0x36,
                   .mnemonic = Mnemonic::ROL,
                   .address_mode = AddressMode::ZPX,
                   .bytes = 2};
  opcodes[0x2e] = {.opcode = 0x2e,
                   .mnemonic = Mnemonic::ROL,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0x3e] = {.opcode = 0x3e,
                   .mnemonic = Mnemonic::ROL,
                   .address_mode = AddressMode::ABSX,
                   .bytes = 3};
  opcodes[0x6a] = {.opcode = 0x6a,
                   .mnemonic = Mnemonic::ROR,
                   .address_mode = AddressMode::ACC,
                   .bytes = 1};
  opcodes[0x66] = {.opcode = 0x66,
                   .mnemonic = Mnemonic::ROR,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0x76] = {.opcode = 0x76,
                   .mnemonic = Mnemonic::ROR,
                   .address_mode = AddressMode::ZPX,
                   .bytes = 2};
  opcodes[0x7e] = {.opcode = 0x7e,
                   .mnemonic = Mnemonic::ROR,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0x6e] = {.opcode = 0x6e,
                   .mnemonic = Mnemonic::ROR,
                   .address_mode = AddressMode::ABSX,
                   .bytes = 3};
  opcodes[0xe9] = {.opcode = 0xe9,
                   .mnemonic = Mnemonic::SBC,
                   .address_mode = AddressMode::IMM,
                   .bytes = 2};
  opcodes[0xe5] = {.opcode = 0xe5,
                   .mnemonic = Mnemonic::SBC,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0xf5] = {.opcode = 0xf5,
                   .mnemonic = Mnemonic::SBC,
                   .address_mode = AddressMode::ZPX,
                   .bytes = 2};
  opcodes[0xed] = {.opcode = 0xed,
                   .mnemonic = Mnemonic::SBC,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0xfd] = {.opcode = 0xfd,
                   .mnemonic = Mnemonic::SBC,
                   .address_mode = AddressMode::ABSX,
                   .bytes = 3};
  opcodes[0xf9] = {.opcode = 0xf9,
                   .mnemonic = Mnemonic::SBC,
                   .address_mode = AddressMode::ABSY,
                   .bytes = 3};
  opcodes[0xe1] = {.opcode = 0xe1,
                   .mnemonic = Mnemonic::SBC,
                   .address_mode = AddressMode::INDX,
                   .bytes = 2};
  opcodes[0xf1] = {.opcode = 0xf1,
                   .mnemonic = Mnemonic::SBC,
                   .address_mode = AddressMode::INDY,
                   .bytes = 2};
  opcodes[0x85] = {.opcode = 0x85,
                   .mnemonic = Mnemonic::STA,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0x95] = {.opcode = 0x95,
                   .mnemonic = Mnemonic::STA,
                   .address_mode = AddressMode::ZPX,
                   .bytes = 2};
  opcodes[0x8d] = {.opcode = 0x8d,
                   .mnemonic = Mnemonic::STA,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0x9d] = {.opcode = 0x9d,
                   .mnemonic = Mnemonic::STA,
                   .address_mode = AddressMode::ABSX,
                   .bytes = 3};
  opcodes[0x99] = {.opcode = 0x99,
                   .mnemonic = Mnemonic::STA,
                   .address_mode = AddressMode::ABSY,
                   .bytes = 3};
  opcodes[0x81] = {.opcode = 0x81,
                   .mnemonic = Mnemonic::STA,
                   .address_mode = AddressMode::INDX,
                   .bytes = 2};
  opcodes[0x91] = {.opcode = 0x91,
                   .mnemonic = Mnemonic::STA,
                   .address_mode = AddressMode::INDY,
                   .bytes = 2};
  opcodes[0x86] = {.opcode = 0x86,
                   .mnemonic = Mnemonic::STX,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0x96] = {.opcode = 0x96,
                   .mnemonic = Mnemonic::STX,
                   .address_mode = AddressMode::ZPY,
                   .bytes = 2};
  opcodes[0x8e] = {.opcode = 0x8e,
                   .mnemonic = Mnemonic::STX,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
  opcodes[0x84] = {.opcode = 0x84,
                   .mnemonic = Mnemonic::STY,
                   .address_mode = AddressMode::ZP,
                   .bytes = 2};
  opcodes[0x94] = {.opcode = 0x94,
                   .mnemonic = Mnemonic::STY,
                   .address_mode = AddressMode::ZPX,
                   .bytes = 2};
  opcodes[0x8c] = {.opcode = 0x8c,
                   .mnemonic = Mnemonic::STY,
                   .address_mode = AddressMode::ABS,
                   .bytes = 3};
}

} // namespace ark
