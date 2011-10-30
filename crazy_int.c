#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>

uint8_t * const MPOOL;
uint8_t * const MSTART_p;
uint8_t * const MEND_p;
uint8_t * HEAP_p;
uint8_t * CONS_SP_p
uint8_t * CONS3_SP_p
uint8_t * CONS3x_SP_p
uint8_t * STR_SP_p;

#define SET_MEMP(ptr, val) (*(uint8_t **)&ptr) = (val)
#define MV_HEAP(bytes) (*(uint8_t**)&HEAP_p) += bytes

#define UBER_2BI_MAX (int16_t)0x1FFF
#define UBER_2BI_MIN (-(UBER_2BI_MAX)-1)

#define UBER_3BI_MAX (int32_t)0x1FFFFF
#define UBER_3BI_MIN (-(UBER_3BI_MAX)-1)

#define MASK2BI ((uint16_t)0x4000)
#define MASK3BI ((uint16_t)0x8000)
#define IS_INT(val) ((val) & 0xF000)
#define IS_2BI(val) ((val) & MASK2BI)
#define IS_3BI(val) ((val) & MASK3BI)

#define TO_14BIT(val) ((uint16_t)(val) & (uint16_t)0x3FFF)
#define TO_2BI(val) (TO_14BIT(val) | MASK2BI)
#define TO_3BH(val) (TO_14BIT((val)>>8) | MASK3BI)
#define TO_3BL(val) ((uint8_t)(val))

#define IS_2BI_COMPAT(val) ((val) <= UBER_2BI_MAX && (val) >= UBER_2BI_MIN)

#define UPTR(ptr) ((uint16_t)((uint8_t *)(ptr) - MPOOL))
#define CPTR(ptr) ((void *)((uint8_t *)(ptr) + MPOOL))

typedef struct {
  uint16_t car;
  uint16_t cdr;
} Cons;

typedef struct {
  uint16_t car;
  uint16_t cdr;
  uint8_t xb;
} Cons3;

typedef struct {
  uint16_t car;
  uint16_t cdr;
  uint8_t car_xb;
  uint8_t cdr_xb;
} Cons3x;

typedef char[17] String;

void *uber_allocate(uint8_t size) {
  *HEAP_p = size;
  void *alloc = HEAP_p + 1;
  MV_HEAP(size+1);
  return alloc;
}

uint16_t cons_alloc() {
  uint8_t *cons_p = CONS_SP_p + (*CONS_SP_p)*sizeof(Cons) + 1;
  *CONS_SP_p++;
  return UPTR(cons_p);
}

uint16_t cons3_alloc() {
  uint8_t *cons3_p = CONS_SP_p + (*CONS3_SP_p)*sizeof(Cons3) + 1;
  *CONS3_SP_p++;
  return UPTR(cons3_p);
}

uint16_t cons3x_alloc() {
  uint8_t *cons3x_p = CONS3x_SP_p + (*CONS3x_SP_p)*sizeof(Cons3x) + 1;
  *CONS3x_SP_p++;
  return UPTR(cons3x_p);
}

uint16_t str_alloc() {
  uint8_t *str_p = STR_SP_p + (*STR_SP_p)*sizeof(String) + 1;
  *STR_SP_p++;
  return UPTR(str_p);
}

uint16_t cons_ptr_ptr(uint16_t car, uint16_t cdr) {
  uint16_t new_cons = cons_alloc();
  new_cons->car = car;
  new_cons->cdr = cdr;
  return new_cons;
}

uint16_t cons_int_ptr(int32_t car, uint16_t cdr) {
  if (IS_2BI_COMPAT(car))
    return cons_ptr_ptr(TO_2BI(car), cdr);

  uint16_t new_cons = cons3_alloc();
  new_cons->car = TO_3BH(car);
  new_cons->xb = TO_3BL(car);
  new_cons->cdr = cdr;

  return new_cons;
}

uint16_t cons_ptr_int(uint16_t car, int32_t cdr) {
  if (IS_2BI_COMPAT(cdr))
    return cons_ptr_ptr(car, TO_2BI(cdr));

  uint16_t new_cons = cons3_alloc();
  new_cons->car = car;
  new_cons->cdr = TO_3BH(cdr);
  new_cons->xb = TO_3BL(cdr);

  return new_cons;
}

uint16_t cons_int_int(int32_t car, int32_t cdr) {
  if (IS_2BI_COMPAT(car)) {
    if (IS_2BI_COMPAT(cdr))
      return cons_ptr_ptr(TO_2BI(car), TO_2BI(cdr));

    return cons_ptr_int(TO_2BI(car), cdr);
  } 

  if (IS_2BI_COMPAT(cdr))
    return cons_int_ptr(car, TO_2BI(cdr));

  uint16_t new_cons = cons3x_alloc();
  new_cons->car = TO_3BH(car);
  new_cons->car_xb = TO_3BL(car);
  new_cons->cdr = TO_3BH(cdr);
  new_cons->cdr_xb = TO_3BL(cdr);

  return new_cons;
 
}

int main() {
  SET_CONSTVP(MPOOL, malloc(0x900));

  /* The first 0x100 addresses are registers in ATmega328 */
  SET_CONSTVP(MSTART_p, MPOOL + 0x100);
  /* 0x800 (2048) bytes of SRAM addresses */
  SET_CONSTVP(MEND_p, MSTART_p + 0x800);

  HEAP_p = MSTART_p;
  CONS_SP_p = uber_allocate(10*sizeof(Cons)+1);
  CONS3_SP_p = uber_allocate(10*sizeof(Cons3)+1);
  CONS3x_SP_p = uber_allocate(10*sizeof(Cons3x)+1);
  STR_SP_p = uber_allocate(10*sizeof(String)+1);

  memset(MPOOL, 0, 0x900);

  return 0;
}
