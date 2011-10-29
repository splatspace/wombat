#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>

void * const MPOOL;
void * const MSTART_p;
void * const MEND_p;
void * HEAP_p;

void * const COBJST_p;
void * const COBJED_p;

#define SET_CONSTVP(ptr, val) (*(void **)&ptr) = (val)
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

#define TO_6BIT(val) ((uint16_t)(val) & (uint16_t)0x3FFF)
#define TO_2BI(val) (TO_6BIT(val) | MASK2BI)
#define TO_3BH(val) (TO_6BIT(((val)>>4)) | MASK3BI)
#define TO_3BL(val) ((uint8_t)(val))

#define IS_2BI_COMPAT(val) ((val) <= UBER_2BI_MAX && (val) >= UBER_2BI_MIN)

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

void *uber_allocate(uint16_t size) {
}

Cons *cons_ptr_ptr(uint16_t car, uint16_t cdr) {
  Cons *new_cons = uber_allocate(sizeof(Cons));
  new_cons->car = car;
  new_cons->cdr = cdr;
  return new_cons;
}

void *cons_int_ptr(int32_t car, uint16_t cdr) {
  if (IS_2BI_COMPAT(car))
    return cons_ptr_ptr(TO_2BI(car), cdr);

  Cons3 *new_cons = uber_allocate(sizeof(Cons3));
  new_cons->car = TO_3BH(car);
  new_cons->xb = TO_3BL(car);
  new_cons->cdr = cdr;

  return new_cons;
}

void *cons_ptr_int(uint16_t car, int32_t cdr) {
  if (IS_2BI_COMPAT(cdr))
    return cons_ptr_ptr(car, TO_2BI(cdr));

  Cons3 *new_cons = uber_allocate(sizeof(Cons3));
  new_cons->car = car;
  new_cons->cdr = TO_3BH(cdr);
  new_cons->xb = TO_3BL(cdr);

  return new_cons;
}

void *cons_int_int(int32_t car, int32_t cdr) {
  if (IS_2BI_COMPAT(car)) {
    if (IS_2BI_COMPAT(cdr))
      return cons_ptr_ptr(TO_2BI(car), TO_2BI(cdr));

    return cons_ptr_int(TO_2BI(car), cdr);
  } 

  if (IS_2BI_COMPAT(cdr))
    return cons_int_ptr(car, TO_2BI(cdr));

  Cons3x *new_cons = uber_allocate(sizeof(Cons3x));
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

  SET_CONSTVP(COBJST_p, MSTART_p);
  SET_CONSTVP(COBJED_p, MSTART_p + 0x100);
  HEAP_p = COBJED_p;

  memset(MPOOL, 0, 0x900);

  return 0;
}
