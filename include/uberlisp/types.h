#ifndef _UBERLISP_TYPES_H
#define _UBERLISP_TYPES_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdarg.h>

typedef volatile intptr_t uptr_t;

#include <avr/pgmspace.h>
#include <uberlisp/gc.h>
#include <uberlisp/symbols.h>

#define UPTR(cptr) ((uptr_t)(cptr))
#define CPTR(uptr) ((void *)TO_PTR((uptr)))

#define LIT_SYM_FLAG (((uint32_t)1)<<31)

#define CADR_FLAG (((uptr_t)1)<<15)
#define INT_FLAG (((uptr_t)1)<<14)
#define GC_FLAG (((uptr_t)1)<<13)

#define IS_CADR(uptr) ((uptr) & CADR_FLAG)
#define VAL(uptr) ((uptr) & ~(CADR_FLAG | GC_FLAG))

#define IS_INT(uptr) ((uptr) & INT_FLAG)
#define IS_PTR(uptr) (!(IS_INT(uptr)))
#define IS_SYM(uptr) (IS_PTR(uptr) && (TO_PTR(uptr) < SEND_p) && (TO_PTR(uptr) >= SSTART_p))
#define IS_CONS(uptr) (IS_PTR(uptr) && (TO_PTR(uptr) >= CSTART_p) && (TO_PTR(uptr) < CEND_p))
#define IS_NIL(uptr) EQ(uptr, NIL)
#define IS_REG(uptr) (IS_PTR(uptr) && !IS_NIL(uptr) && (TO_PTR(uptr) < 0x100))

#define TO_PTR(uptr) ((uptr) & 0x1FFF)
#define TO_INT(uptr) (((int16_t)((uptr)<<3))>>3)

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
#define CADAR(uptr) (CAR(CDAR(uptr)))

#define SVAL(symptr) (*SYM_PTR(symptr))

#define EQ(uptr1, uptr2) (VAL(uptr1) == VAL(uptr2))

typedef struct {
  uptr_t car;
  uptr_t cdr;
} Cons;

#define NIL ((uptr_t)0)

uint32_t hash_sym(char *name);
void unhash_sym(char *buf, uptr_t sym_p);

extern char __heap_start;
extern char __bss_end;

#define PTR_CACHE_SIZE 128

extern uptr_t CSTART_p;
extern uptr_t CEND_p;
extern uptr_t SSTART_p;
extern uptr_t SEND_p;
extern uptr_t *PTREND_p;

#define TOTALMEM() (CEND_p    - SSTART_p)
#define FREEMEM()  (CSTART_p  - SEND_p)
#define CONSMEM()  (CEND_p    - CSTART_p)
#define SYMMEM()   (SEND_p    - SSTART_p)
#define USEDMEM()  (CONSMEM() + SYMMEM())

void init_mem();
uptr_t build_cons(uptr_t car, uptr_t cdr);
uptr_t build_list(uptr_t car, ...);
static inline void __mk_sym(uint32_t s) {
  SVAL(SEND_p) = s;
  SEND_p += 4;
}
uptr_t build_symbol(char *name);

static inline uptr_t *refer(uptr_t uptr) {
  *PTREND_p = uptr;
  return PTREND_p++;
}

static inline void release(int ptr_count) {
  PTREND_p -= ptr_count;
}

#endif
