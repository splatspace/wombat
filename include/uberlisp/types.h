#ifndef _UBERLISP_TYPES_H
#define _UBERLISP_TYPES_H

#include <stdint.h>

typedef intptr_t uptr_t;

#define UPTR(cptr) ((uptr_t)(cptr))
#define CPTR(uptr) ((void *)(uptr))

#define LIT_SYM_FLAG (((uint32_t)1)<<31)

#define CADR_FLAG (((uptr_t)1)<<15)
#define INT_FLAG (((uptr_t)1)<<14)

#define IS_CADR(uptr) ((uptr) & CADR_FLAG)
#define VAL(uptr) ((uptr) & ~CADR_FLAG)

#define IS_INT(uptr) ((uptr) & INT_FLAG)
#define IS_PTR(uptr) (!(IS_INT(uptr)))
#define IS_SYM(uptr) (IS_PTR(uptr) && (TO_PTR(uptr) < SEND_p))
#define IS_CONS(uptr) (IS_PTR(uptr) && (TO_PTR(uptr) >= CSTART_p))
#define IS_NIL(uptr) EQ(uptr, NIL)
#define IS_REG(uptr) (IS_PTR(uptr) && (TO_PTR(uptr) < 0x100))

#define TO_PTR(uptr) ((uptr) & 0x3FFF)
#define TO_INT(uptr) (((int16_t)((uptr)<<2))>>2)

#define INTERN_INT(val) (TO_PTR(val) | INT_FLAG)

#define UPTR_PTR(uptr) ((uptr_t *)CPTR(uptr))
#define BYTE_PTR(uptr) ((uint8_t *)CPTR(uptr))
#define SYM_PTR(uptr) ((uint32_t *)CPTR(uptr))
#define CONS_PTR(uptr) ((Cons *)CPTR(uptr))

#define CAR(uptr) (VAL(CONS_PTR(uptr)->car))
#define __CDR(uptr) (CONS_PTR(uptr)->cdr)
#define CDR(uptr) (IS_CADR(__CDR(uptr)) ? (VAL(uptr) + sizeof(uptr_t)) : __CDR(uptr))

#define CAAR(uptr) (CAR(CAR(uptr)))
#define CADR(uptr) (CAR(CDR(uptr)))
#define CDAR(uptr) (CDR(CAR(uptr)))
#define CDDR(uptr) (CDR(CDR(uptr)))
#define CADDR(uptr) (CAR(CDDR(uptr)))
#define CDDDR(uptr) (CDR(CDDR(uptr)))

#define SVAL(symptr) (*SYM_PTR(symptr))

#define EQ(uptr1, uptr2) (VAL(uptr1) == VAL(uptr2))

typedef struct { 
  uptr_t car;
  uptr_t cdr;
} Cons;

#define NIL ((uptr_t)0)

#define USCORE_HSH 27

uint32_t hash_sym(char *name);
void unhash_sym(char *buf, uptr_t sym_p);

extern char __heap_start;
extern char __bss_end;

uptr_t CSTART_p;
uptr_t CEND_p;
uptr_t SSTART_p;
uptr_t SEND_p;

void init_mem();
uptr_t build_cons(uptr_t car, uptr_t cdr);
uptr_t build_symbol(char *name);

#endif
