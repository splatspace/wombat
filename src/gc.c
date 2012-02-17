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
      if (IS_CONS(*cur)) {
        uptr_t *cons = UPTR_PTR(*cur);
        *cons |= GC_FLAG;
        while (IS_CADR(*(++cons)));
        *cons |= GC_FLAG;
      }
      cur++;
    }
    printf_P(PSTR("Done!\n"));

    uptr_t *free_st, *free_end;
    cur = UPTR_PTR(CSTART_p);
    printf_P(PSTR("Sweeping...\n"));
    while (cur < UPTR_PTR(CEND_p)) {
      if (! (*cur & (GC_FLAG | CADR_FLAG))) {
        garbage = 1;
        free_st = cur;

        while (! (*cur & GC_FLAG) && cur < UPTR_PTR(CEND_p)) cur++;

        if (IS_CADR(*cur)) *cur &= ~CADR_FLAG;

        free_end = cur;

        int free_bytes = (intptr_t)free_end - (intptr_t)free_st,
          kept_bytes = (intptr_t)free_st - CSTART_p;

        printf_P(PSTR("Freeing %i bytes...\n"), free_bytes);
        printf_P(PSTR("free_st: %p\t free_end: %p\n"), free_st, free_end);
                 
        memmove(CPTR(CSTART_p + free_bytes), CPTR(CSTART_p), kept_bytes);
        memset(CPTR(CSTART_p), 0, free_bytes);
        CSTART_p += free_bytes;

        for (cur = UPTR_PTR(CSTART_p); cur < PTREND_p; ++cur)
          if (IS_CONS(*cur) && TO_PTR(*cur) < UPTR(free_end)) {
            printf_P(PSTR("Adjusting pointer (%p)\told: 0x%x (0x%x)\t"), cur, TO_PTR(*cur), *cur);
            *cur += free_bytes;
            printf_P(PSTR("new: 0x%x (0x%x)\n"), TO_PTR(*cur), *cur);
          }
        cur = free_end;

      } else
        cur++;
    }
    printf_P(PSTR("Done!\n"));
    
    printf_P(PSTR("Clearing GC bits..."));
    for (cur = UPTR_PTR(CSTART_p); cur < UPTR_PTR(CEND_p); ++cur)
      *cur &= ~GC_FLAG;
    printf_P(PSTR("Done!\n"));

  } while (garbage);
  printf_P(PSTR("GC Complete!\n"));
  print_mem();
}
