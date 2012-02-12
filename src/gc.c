#include <uberlisp/gc.h>

void __GC__() {
  int garbage;
  do {
    garbage = 0;
    uptr_t *cur = UPTR_PTR(CSTART_p);
    while (cur < PTREND_p) {
      if (IS_CONS(*cur)) *UPTR_PTR(*cur) |= GC_FLAG;
      cur++;
    }

    uptr_t *free_st, *free_end;
    cur = UPTR_PTR(CSTART_p);
    while (cur < UPTR_PTR(CEND_p)) {
      if (! (*cur & GC_FLAG)) {
        garbage = 1;
        free_st = cur;
        do {
          ++cur;
        } while (IS_CADR(*cur));
        ++cur;
        free_end = cur;
        int free_bytes = (intptr_t)free_end - (intptr_t)free_st;
        memmove(CPTR(CSTART_p + free_bytes), CPTR(CSTART_p), free_bytes);
        memset(CPTR(CSTART_p), 0, free_bytes);
        CSTART_p += free_bytes;
        for (; cur < PTREND_p; ++cur)
          if (IS_CONS(*cur) && TO_PTR(*cur) < CSTART_p) *cur += free_bytes;
        cur = free_end;
      }
    }

    for (cur = UPTR_PTR(CSTART_p); cur < UPTR_PTR(CEND_p); ++cur)
      *cur &= ~GC_FLAG;
  } while (garbage);
}
