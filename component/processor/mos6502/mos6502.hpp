#pragma once

#include <platform/platform.hpp>

// #define CONST_JUMP_TABLE

struct MOS6502 {
  auto execute(s64) -> s64;

  struct IO {
    bool res; // reset
    bool nmi; // non-maskable interrupt
    bool irq; // interrupt request
  } io;

private:
  auto read(u16) -> u8;

private:
  static void *opcodeTatalTable;
  static void *opcodeJumpTable[256];

  s64 cycles;

  u16 addressBus;

  // internal registers
  u8 a, x, y, s;
  u16 pc;

  struct StatusRegister {
    bool c; // Carry
    bool z; // Zero
    bool i; // Interrupt Disable
    bool d; // Decimal Mode
    bool b; // Break Command
    // bool u; // Unused
    bool v; // Overflow
    bool n; // Negative

    operator u8() const {
      return c << 0 | z << 1 | i << 2 | d << 3 | b << 4 | 1 << 5 | v << 6 | n << 7;
    }

    auto &operator=(u8 v) {
      c = v >> 0 & 1;
      z = v >> 1 & 1;
      i = v >> 2 & 1;
      d = v >> 3 & 1;
      b = v >> 4 & 1;
      v = v >> 6 & 1;
      n = v >> 7 & 1;
      return *this;
    }
  } p;
};
