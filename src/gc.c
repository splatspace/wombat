#include <uberlisp/gc.h>

void __GC__() {
  int garbage;
  do {
    garbage = 0;

    uptr_t *cur = UPTR_PTR(CSTART_p);
    while (cur < PTREND_p) {
      if (IS_CONS(*cur)) {
        uptr_t *cons = UPTR_PTR(*cur);
        *cons |= GC_FLAG;
        while (IS_CADR(*(++cons)));
        *cons |= GC_FLAG;
      }
      cur++;
    }

    uptr_t *free_st, *free_end;
    cur = UPTR_PTR(CSTART_p);
    while (cur < UPTR_PTR(CEND_p)) {
      if (! (*cur & (GC_FLAG | CADR_FLAG))) {
        garbage = 1;
        free_st = cur;

        while (! (*cur & GC_FLAG) && cur < UPTR_PTR(CEND_p)) cur++;

        *cur &= ~CADR_FLAG; // If CADR, it isn't anymore. No need to check; just adds insxns.

        free_end = cur;

        int free_bytes = (intptr_t)free_end - (intptr_t)free_st,
          kept_bytes = (intptr_t)free_st - CSTART_p;

        memmove(CPTR(CSTART_p + free_bytes), CPTR(CSTART_p), kept_bytes);
        memset(CPTR(CSTART_p), 0, free_bytes);

        for (cur = UPTR_PTR(CSTART_p + free_bytes); cur < PTREND_p; ++cur)
          if (IS_CONS(*cur) && TO_PTR(*cur) < UPTR(free_end)) {
            // We should assert that none of the pointers we're
            // shifting are pointing to the freed memory location
            // (btwn free_st & free_end)
            *cur += free_bytes;
          }
        CSTART_p += free_bytes; // IS_CONS() depends on CSTART_p, can't move until after we fix ptrs
        cur = free_end;

      } else
        cur++;
    }

    for (cur = UPTR_PTR(CSTART_p); cur < UPTR_PTR(CEND_p); ++cur)
      *cur &= ~GC_FLAG;

  } while (garbage);
}
