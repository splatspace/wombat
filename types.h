#ifndef _UBERLISP_TYPES_H
#define _UBERLISP_TYPES_H

#define INT_FLAG (1<<14)
#define CADR_FLAG (1<<15)

#define IS_INT(uptr) ((uptr) & INT_FLAG)
#define IS_PTR(uptr) (!(IS_INT(uptr)))

#define TO_PTR(uptr) ((uptr) & 0x3FFF)

typedef uint16_t uptr_t;

typedef struct { 
  uptr_t car;
  uptr_t cdr;
} Cons;

#endif
