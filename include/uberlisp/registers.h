#ifndef _UBERLISP_REGISTERS_H

#include <avr/io.h>
#include <avr/pgmspace.h>

typedef struct {
  uint32_t symhsh;
  uint8_t reg;
} Reg;

#define REGC 2
uint8_t __regmap[REGC][sizeof(Reg)] = {
  { 'D', 'D', 'R', ('B' | 0x80), (uint8_t)(0x04 + __SFR_OFFSET) },
  { 'P', 'R', 'T', ('B' | 0x80), (uint8_t)(0x05 + __SFR_OFFSET) }
};

Reg *Regmap = (Reg*)&__regmap;
#define REGEND (Regmap + REGC)
#define REG_PTR(rg) ((uint8_t*)(((intptr_t)0) | rg->reg))

#endif
