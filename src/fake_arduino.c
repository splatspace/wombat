#include <stdlib.h>
#include <string.h>

#include <uberlisp/fake_arduino.h>

void init_env() {
  SET_MEMP(MPOOL, malloc(0x900));
  memset(MPOOL, 0, 0x900);

  /* The first 0x100 addresses are registers in ATmega328 */
  SET_MEMP(MSTART_p, MPOOL + 0x100);
  /* 0x800 (2048) bytes of SRAM addresses */
  SET_MEMP(MEND_p, MSTART_p + 0x800);

  HEAP_p = MSTART_p;
}
