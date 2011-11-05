#ifndef _FAKE_ARDUINO_H
#define _FAKE_ARDUINO_H

#include <stdint.h>

uint8_t * const MPOOL;
uint8_t * const MSTART_p;
uint8_t * const MEND_p;
uint8_t * HEAP_p;

#define SET_MEMP(ptr, val) (*(uint8_t **)&ptr) = (val)

void init_env();

#endif
