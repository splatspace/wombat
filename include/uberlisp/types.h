#ifndef _UBERLISP_TYPES_H
#define _UBERLISP_TYPES_H

#include <stdint.h>

#include <uberlisp/uberlisp.h>

#define LIT_SYM_FLAG (((uint32_t)1)<<31)

#define INT_FLAG (((uint16_t)1)<<14)
#define CADR_FLAG (((uint16_t)1)<<15)

#define IS_CADR(uptr) ((uptr) & CADR_FLAG)
#define VAL(uptr) ((uptr) & ~CADR_FLAG)

#define IS_INT(uptr) ((uptr) & INT_FLAG)
#define IS_PTR(uptr) (!(IS_INT(uptr)))
#define IS_SYM(uptr) (IS_PTR(uptr) && (TO_PTR(uptr) < SEND_p))
#define IS_CONS(uptr) (IS_PTR(uptr) && (TO_PTR(uptr) >= CSTART_p))

#define TO_PTR(uptr) ((uptr) & 0x3FFF)
#define TO_INT(uptr) (((int16_t)((uptr)<<2))>>2)

#define INTERN_INT(val) (TO_PTR(val) | INT_FLAG)

#define UPTR_PTR(uptr) ((uptr_t *)CPTR(uptr))
#define BYTE_PTR(uptr) ((uint8_t *)CPTR(uptr))
#define SYM_PTR(uptr) ((uint32_t *)CPTR(uptr))
#define CONS_PTR(uptr) ((Cons *)CPTR(uptr))

#define CAR(uptr) (VAL(CONS_PTR(uptr)->car))
#define __CDR(uptr) (CONS_PTR(uptr)->cdr)
#define CDR(uptr) (IS_CADR(__CDR(uptr)) ? VAL((uptr) + sizeof(uptr_t)) : __CDR(uptr))

typedef uint16_t uptr_t;

typedef struct { 
  uptr_t car;
  uptr_t cdr;
} Cons;

#define NIL ((uptr_t)0)

#define USCORE_HSH 27

void hash_sym(uptr_t store, char *name);
void unhash_sym(char *buf, uptr_t sym_p);

uptr_t CSTART_p;
uptr_t CEND_p;
uptr_t SSTART_p;
uptr_t SEND_p;

void init_mem();
uptr_t build_cons(uptr_t car, uptr_t cdr);
uptr_t build_symbol(char *name);

#endif
