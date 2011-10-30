#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>

uint8_t * const MPOOL;
uint8_t * const MSTART_p;
uint8_t * const MEND_p;
uint8_t * HEAP_p;
uint8_t * CONS_SP_p;
uint8_t * CONS3_SP_p;
uint8_t * CONS3x_SP_p;
uint8_t * STR_SP_p;

#define SET_MEMP(ptr, val) (*(uint8_t **)&ptr) = (val)

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

#define UPTR(ptr) ((uint16_t)((uintptr_t)(ptr) - (uintptr_t)MPOOL))
#define CPTR(uptr) ((void *)((uintptr_t)(uptr) + (uintptr_t)MPOOL))

#define STR_SIZE 17

#define IS_IN_SPACE(uptr, sp, size) ((uptr) > UPTR(sp) && (uptr) < UPTR(sp + (*sp)*size+1))
#define IS_STR(uptr) (IS_IN_SPACE(uptr, STR_SP_p, STR_SIZE))
#define IS_CONS(uptr) (IS_IN_SPACE(uptr, CONS_SP_p, sizeof(Cons)) || IS_IN_SPACE(uptr, CONS3_SP_p, sizeof(Cons3)) || IS_IN_SPACE(uptr, CONS3x_SP_p, sizeof(Cons3x)))

#define CAR(uptr) (((Cons *)CPTR(uptr))->car)
#define CDR(uptr) (((Cons *)CPTR(uptr))->cdr)
#define XB(uptr) (((Cons3 *)CPTR(uptr))->xb)
#define XB_CDR(uptr) (((Cons3x *)CPTR(uptr))->xb_cdr)

#define IVAL_2BI(val) ((int16_t)((uint16_t)(val)<<2)>>2)
#define IVAL_3BI(val_h, val_l) ((((int32_t)IVAL_2BI(val_h))<<8) | (uint32_t)(val_l))

#define NIL 0

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
  uint8_t xb;
  uint8_t xb_cdr;
} Cons3x;

uint8_t *uber_allocate(uint8_t size) {
  *HEAP_p = size;
  uint8_t *alloc = HEAP_p + 1;
  HEAP_p += size+1;
  return alloc;
}

uint16_t cons_alloc() {
  uint8_t *cons_p = CONS_SP_p + (*CONS_SP_p)*sizeof(Cons) + 1;
  (*CONS_SP_p)++;
  return UPTR(cons_p);
}

uint16_t cons3_alloc() {
  uint8_t *cons3_p = CONS3_SP_p + (*CONS3_SP_p)*sizeof(Cons3) + 1;
  (*CONS3_SP_p)++;
  return UPTR(cons3_p);
}

uint16_t cons3x_alloc() {
  uint8_t *cons3x_p = CONS3x_SP_p + (*CONS3x_SP_p)*sizeof(Cons3x) + 1;
  (*CONS3x_SP_p)++;
  return UPTR(cons3x_p);
}

uint16_t str_alloc() {
  uint8_t *str_p = STR_SP_p + (*STR_SP_p)*STR_SIZE + 1;
  (*STR_SP_p)++;
  return UPTR(str_p);
}

uint16_t cons_ptr_ptr(uint16_t car, uint16_t cdr) {
  uint16_t new_cons = cons_alloc();
  ((Cons *)CPTR(new_cons))->car = car;
  ((Cons *)CPTR(new_cons))->cdr = cdr;
  return new_cons;
}

uint16_t cons_int_ptr(int32_t car, uint16_t cdr) {
  if (IS_2BI_COMPAT(car))
    return cons_ptr_ptr(TO_2BI(car), cdr);

  uint16_t new_cons = cons3_alloc();
  ((Cons3 *)CPTR(new_cons))->car = TO_3BH(car);
  ((Cons3 *)CPTR(new_cons))->xb = TO_3BL(car);
  ((Cons3 *)CPTR(new_cons))->cdr = cdr;

  return new_cons;
}

uint16_t cons_ptr_int(uint16_t car, int32_t cdr) {
  if (IS_2BI_COMPAT(cdr))
    return cons_ptr_ptr(car, TO_2BI(cdr));

  uint16_t new_cons = cons3_alloc();
  ((Cons3 *)CPTR(new_cons))->car = car;
  ((Cons3 *)CPTR(new_cons))->cdr = TO_3BH(cdr);
  ((Cons3 *)CPTR(new_cons))->xb = TO_3BL(cdr);

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
  ((Cons3x *)CPTR(new_cons))->car = TO_3BH(car);
  ((Cons3x *)CPTR(new_cons))->xb = TO_3BL(car);
  ((Cons3x *)CPTR(new_cons))->cdr = TO_3BH(cdr);
  ((Cons3x *)CPTR(new_cons))->xb_cdr = TO_3BL(cdr);

  return new_cons;
 
}

void print_form(uint16_t);

void print_list(uint16_t list_p) {
  if (IS_2BI(CAR(list_p)))
    printf("%d", IVAL_2BI(CAR(list_p)));
  else if (IS_3BI(CAR(list_p)))
    printf("%d", IVAL_3BI(CAR(list_p), XB(list_p)));
  else
    print_form(CAR(list_p));

  if (IS_CONS(CDR(list_p))) {
    printf(" ");
    print_list(CDR(list_p));
  } else if (CDR(list_p) != NIL) {
    printf(" . ");
    if (IS_2BI(CDR(list_p)))
      printf("%d", IVAL_2BI(CDR(list_p)));
    else if (IS_3BI(CDR(list_p)))
      printf("%d", IVAL_3BI(CDR(list_p), (IS_3BI(CAR(list_p)) ? XB_CDR(list_p) : XB(list_p))));
    else
      print_form(CDR(list_p));
  }
}

void print_form(uint16_t form) {
  if (IS_CONS(form)) {
    printf("(");
    print_list(form);
    printf(")");
  } else if (IS_STR(form)) {
    printf("%s", (char *)CPTR(form));
  } else if (IS_INT(form)) {
    printf("%s%d", (IS_3BI(form) ? "<trunc>" : ""), IVAL_2BI(form));
  } else {
    printf("WUT?");
  }
}

int main() {
  SET_MEMP(MPOOL, malloc(0x900));
  memset(MPOOL, 0, 0x900);

  /* The first 0x100 addresses are registers in ATmega328 */
  SET_MEMP(MSTART_p, MPOOL + 0x100);
  /* 0x800 (2048) bytes of SRAM addresses */
  SET_MEMP(MEND_p, MSTART_p + 0x800);

  HEAP_p = MSTART_p;
  CONS_SP_p = uber_allocate(20*sizeof(Cons)+1);
  CONS3_SP_p = uber_allocate(20*sizeof(Cons3)+1);
  CONS3x_SP_p = uber_allocate(20*sizeof(Cons3x)+1);
  STR_SP_p = uber_allocate(10*STR_SIZE+1);

  uint16_t list = 
    cons_int_ptr(UBER_3BI_MIN,
        cons_int_ptr(UBER_3BI_MAX,
          cons_int_ptr(UBER_2BI_MIN-1,
            cons_int_ptr(UBER_2BI_MAX+1,
              cons_int_ptr(UBER_2BI_MIN,
                cons_int_ptr(UBER_2BI_MAX,
                  cons_int_ptr(-42,
                    cons_int_ptr(42, NIL))))))));

  print_form(list);
  printf("\n");

  uint16_t lol = cons_ptr_int(list, 500);
  print_form(lol);
  printf("\n");
  print_form(cons_ptr_ptr(list, list));
  printf("\n");
  print_form(cons_int_int(0, UBER_3BI_MAX));
  printf("\n");
  print_form(cons_int_int(0, 65535));
  printf("\n");

  return 0;
}
