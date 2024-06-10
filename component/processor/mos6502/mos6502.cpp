#include "component/processor/mos6502/mos6502.hpp"

#include <stdlib.h>

auto MOS6502::execute(s64 cycles) -> s64 {
  u8 step = 0;

  if (opcodeTatalTable == nullptr) {
    opcodeTatalTable = malloc(256 * sizeof(void*));

#ifdef BUILD_DEBUG
    
#else
    // opcodeJumpTable = new void*[256];
#endif
  }

  return 0;
}
