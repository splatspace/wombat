#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

void * const MPOOL;
void * const MSTART_p;
void * const MEND_p;
void * HEAP_p;

#define SET_CONSTVP(ptr, val) (*(void **)&ptr) = (val)

#define INT2_T (unsigned char)0x20
#define INT4_T (unsigned char)0x40
#define INT8_T (unsigned char)0x80

#define SET_INT_T(ptr, type) (*(unsigned char*)ptr) = type
#define MV_HEAP(bytes) (*(unsigned char**)&HEAP_p) += bytes

typedef struct {
  short val_p;
} Object;

void *store_int(long value) {
  void *new_int_p = HEAP_p;
  if (value <= SHRT_MAX && value >= SHRT_MIN) {
    SET_INT_T(HEAP_p, INT2_T);
    MV_HEAP(1);
    *(short*)HEAP_p = (short)value;
    MV_HEAP(sizeof(short));
  } else if (value <= INT_MAX && value >= INT_MIN) {
    SET_INT_T(HEAP_p, INT4_T);
    MV_HEAP(1);
    *(int*)HEAP_p = (int)value;
    MV_HEAP(sizeof(int));
  } else {
    SET_INT_T(HEAP_p, INT8_T);
    MV_HEAP(1);
    *(long*)HEAP_p = value;
    MV_HEAP(sizeof(long));
  }

  return new_int_p;
}

#define IS_INT2(int_p) (*(unsigned char*)(int_p) & INT2_T)
#define IS_INT4(int_p) (*(unsigned char*)(int_p) & INT4_T)
#define IS_INT8(int_p) (*(unsigned char*)(int_p) & INT8_T)

long read_int(void *int_p) {
  void *val_p = (unsigned char*)int_p + 1;
  if (IS_INT2(int_p)) return (long)(*(short*)val_p);
  if (IS_INT4(int_p)) return (long)(*(int*)val_p);
  return *(long*)val_p;
}

int main() {
  SET_CONSTVP(MPOOL, malloc(0x900));
  SET_CONSTVP(MSTART_p, MPOOL + 0x100);
  SET_CONSTVP(MEND_p, MPOOL + 0x900);
  HEAP_p = MSTART_p;

  memset(MSTART_p, 0, 0x800);

  void *sp1 = store_int(0);
  void *sp2 = store_int(-1);
  void *sp3 = store_int(SHRT_MAX);
  void *sp4 = store_int(SHRT_MIN);

  void *ip1 = store_int(SHRT_MAX + 1);
  void *ip2 = store_int(SHRT_MIN - 1);
  void *ip3 = store_int(INT_MAX);
  void *ip4 = store_int(INT_MIN);

  void *lp1 = store_int((long)INT_MAX + 1);
  void *lp2 = store_int((long)INT_MIN - 1);
  void *lp3 = store_int(LONG_MAX);
  void *lp4 = store_int(LONG_MIN);

  printf("%li should be 0\n", read_int(sp1));
  printf("%li should be -1\n", read_int(sp2));
  printf("%li should be %d\n", read_int(sp3), SHRT_MAX);
  printf("%li should be %d\n", read_int(sp4), SHRT_MIN);
  printf("%li should be %d\n", read_int(ip1), SHRT_MAX+1);
  printf("%li should be %d\n", read_int(ip2), SHRT_MIN-1);
  printf("%li should be %d\n", read_int(ip3), INT_MAX);
  printf("%li should be %d\n", read_int(ip4), INT_MIN);
  printf("%li should be %li\n", read_int(lp1), (long)INT_MAX+1);
  printf("%li should be %li\n", read_int(lp2), (long)INT_MIN-1);
  printf("%li should be %li\n", read_int(lp3), LONG_MAX);
  printf("%li should be %li\n", read_int(lp4), LONG_MIN);

  return 0;
}
