#include "component/processor/mos6502/mos6502.hpp"

auto MOS6502::address() {

Absolute:
  addressBus = read(pc++);
  addressBus = read(pc++) << 8;


  return 0;
}
