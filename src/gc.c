#include <uberlisp/gc.h>

void __GC__() {
  printf_P(PSTR("Beginning GC...\n"));
  int garbage;
  do {
    printf_P(PSTR("Top of garbage loop.\n"));
    garbage = 0;
    uptr_t *cur = UPTR_PTR(CSTART_p);
    printf_P(PSTR("Marking..."));
    while (cur < PTREND_p) {
      if (IS_CONS(*cur)) *UPTR_PTR(*cur) |= GC_FLAG;
      cur++;
    }
    printf_P(PSTR("Done!\n"));

    uptr_t *free_st, *free_end;
    cur = UPTR_PTR(CSTART_p);
    printf_P(PSTR("Sweeping...\n"));
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
        printf_P(PSTR("Freeing %i bytes...\n"), free_bytes);
        memmove(CPTR(CSTART_p + free_bytes), CPTR(CSTART_p), free_bytes);
        memset(CPTR(CSTART_p), 0, free_bytes);
        CSTART_p += free_bytes;
        for (; cur < PTREND_p; ++cur)
          if (IS_CONS(*cur) && TO_PTR(*cur) < CSTART_p) *cur += free_bytes;
        cur = free_end;
      } else
        ++cur;
    }
    printf_P(PSTR("Done!\n"));
    
    printf_P(PSTR("Clearing GC bits..."));
    for (cur = UPTR_PTR(CSTART_p); cur < UPTR_PTR(CEND_p); ++cur)
      *cur &= ~GC_FLAG;
    printf_P(PSTR("Done!\n"));
  } while (garbage);
  printf_P(PSTR("GC Complete!\n"));
}
