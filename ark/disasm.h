#pragma once

#include <cstdint>
#include <string>

namespace ark {

std::string disassemble(uint8_t opcode, uint8_t operand1, uint8_t operand2);

enum class Mnemonic {
  ILLEGAL,
  ADC,
  AND,
  ASL,
  BCC,
  BCS,
  BEQ,
  BIT,
  BMI,
  BNE,
  BPL,
  BRK,
  BVC,
  BVS,
  CLC,
  CLD,
  CLI,
  CLV,
  CMP,
  CPX,
  CPY,
  DEC,
  DEX,
  DEY,
  EOR,
  INC,
  INX,
  INY,
  JMP,
  JSR,
  LDA,
  LDX,
  LDY,
  LSR,
  NOP,
  ORA,
  PHA,
  PHP,
  PLA,
  PLP,
  ROL,
  ROR,
  RTI,
  RTS,
  SBC,
  SEC,
  SED,
  SEI,
  STA,
  STX,
  STY,
  TAX,
  TAY,
  TSX,
  TXA,
  TXS,
  TYA,
};

enum class AddressMode {
  ILLEGAL,
  ABS,
  ABSX,
  ABSY,
  ACC,
  IMM,
  IMP,
  IND,
  INDX,
  INDY,
  REL,
  ZP,
  ZPX,
  ZPY
};

struct OpcodeInfo {
  uint8_t opcode;
  Mnemonic mnemonic;
  AddressMode address_mode;
  uint8_t bytes;
};

} // namespace ark
