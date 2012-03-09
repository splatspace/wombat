#ifndef _UBERLISP_SYMBOLS_H
#define _UBERLISP_SYMBOLS_H

#include <avr/io.h>
#include <uberlisp/types.h>
#include <uberlisp/alist.h>

#define USCORE_HSH 27

#define __SHASH4(c1, c2, c3, c4) \
  (((uint32_t)0)                 \
   | ((uint32_t)c1)              \
   | (((uint32_t) c2)<<8)        \
   | (((uint32_t) c3)<<16)       \
   | (((uint32_t) c4)<<24)       \
   | LIT_SYM_FLAG)

#define __CHRNUM(c) ((uint32_t)(c - 'A' + 1))

#define __SHASH6(c1, c2, c3, c4, c5, c6) \
  (((uint32_t) 0)                        \
   | (__CHRNUM(c1))                      \
   | (__CHRNUM(c2)<<5)                   \
   | (__CHRNUM(c3)<<10)                  \
   | (__CHRNUM(c4)<<15)                  \
   | (__CHRNUM(c5)<<20)                  \
   | (__CHRNUM(c6)<<25))

#define S_LET   __SHASH4('L', 'E', 'T',  0)
#define S_FN    __SHASH4('F', 'N',  0,   0)
#define S_DO    __SHASH4('D', 'O',  0,   0)
#define S_CAR   __SHASH4('C', 'A', 'R',  0)
#define S_CDR   __SHASH4('C', 'D', 'R',  0)
#define S_CONS  __SHASH4('C', 'O', 'N', 'S')
#define S_DEF   __SHASH4('D', 'E', 'F',  0)
#define S_IF    __SHASH4('I', 'F',  0,   0)
#define S_AND   __SHASH4('A', 'N', 'D',  0)
#define S_OR    __SHASH4('O', 'R',  0,   0)
#define S_NOT   __SHASH4('N', 'O', 'T',  0)
#define S_TRUE  __SHASH4('T',  0,   0,   0)
#define S_EVAL  __SHASH4('E', 'V', 'A', 'L')
#define S_PLUS  __SHASH4('+',  0,   0,   0)
#define S_MINUS __SHASH4('-',  0,   0,   0)
#define S_MULT  __SHASH4('*',  0,   0,   0)
#define S_DIV   __SHASH4('/',  0,   0,   0)
#define S_EQL   __SHASH4('=',  0,   0,   0)
#define S_NEQL  __SHASH4('N', 'O', 'T', '=')
#define S_LT    __SHASH4('<',  0,   0,   0)
#define S_LTE   __SHASH4('<',  '=', 0,   0)
#define S_GT    __SHASH4('>',  0,   0,   0)
#define S_GTE   __SHASH4('>',  '=', 0,   0)
#define S_SREG  __SHASH4('S', 'R', 'E', 'G')
#define S_SLP   __SHASH4('S', 'L', 'P',  0)
#define S_LOOP  __SHASH4('L', 'O', 'O', 'P')
#define S_WHEN  __SHASH4('W', 'H', 'E', 'N')

#define _Z_ ('A' - 1) // hashes to 0
#define _U_ ('A' + USCORE_HSH - 1) // hashes to USCORE_HSH

#define S_PRINT __SHASH6('P', 'R', 'I', 'N', 'T', _Z_)
#define S_QUOTE __SHASH6('Q', 'U', 'O', 'T', 'E', _Z_)
#define S_RECUR __SHASH6('R', 'E', 'C', 'U', 'R', _Z_)

#define S__DDRB  __SHASH6(_U_, 'D', 'D', 'R', 'B', _Z_)
#define S__PORTB __SHASH6(_U_, 'P', 'O', 'R', 'T', 'B')

#define PS_LET   (SSTART_p + 0x0)
#define PS_FN    (SSTART_p + 0x4)
#define PS_DO    (SSTART_p + 0x8)
#define PS_QUOTE (SSTART_p + 0xC)
#define PS_CAR   (SSTART_p + 0x10)
#define PS_CDR   (SSTART_p + 0x14)
#define PS_CONS  (SSTART_p + 0x18)
#define PS_PRINT (SSTART_p + 0x1C)
#define PS_DEF   (SSTART_p + 0x20)
#define PS_IF    (SSTART_p + 0x24)
#define PS_AND   (SSTART_p + 0x28)
#define PS_OR    (SSTART_p + 0x2C)
#define PS_NOT   (SSTART_p + 0x30)
#define PS_TRUE  (SSTART_p + 0x34)
#define PS_EVAL  (SSTART_p + 0x38)
#define PS_PLUS  (SSTART_p + 0x3C)
#define PS_MINUS (SSTART_p + 0x40)
#define PS_MULT  (SSTART_p + 0x44)
#define PS_DIV   (SSTART_p + 0x48)
#define PS_EQL   (SSTART_p + 0x4C)
#define PS_NEQL  (SSTART_p + 0x50)
#define PS_LT    (SSTART_p + 0x54)
#define PS_LTE   (SSTART_p + 0x58)
#define PS_GT    (SSTART_p + 0x5C)
#define PS_GTE   (SSTART_p + 0x60)
#define PS_SREG  (SSTART_p + 0x64)
#define PS_SLP   (SSTART_p + 0x68)
#define PS_LOOP  (SSTART_p + 0x6C)
#define PS_RECUR (SSTART_p + 0x70)
#define PS_WHEN  (SSTART_p + 0x74)

void init_syms(uptr_t *env);

#endif
