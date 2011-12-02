#include <uberlisp/gc.h>
#include <uberlisp/print_form.h>

#define IS_GARBAGE(uptr) (!(GC_FLAG & uptr))

int __GC__() {
  printf_P(PSTR("\n** BEGIN GC **\n"));
  uptr_t *cur, *orig_CSTART_p = CSTART_p;
  int garbage;

  do {
    uptr_t *iter_CSTART_p = CSTART_p;
    
    uptr_t *cadr;
    for(cadr = UPTR_PTR(ENV_p); IS_CADR(*cadr); cadr++) 
      *cadr |= GC_FLAG;
    *cadr |= GC_FLAG;

    for (cur = CSTART_p; cur < CEND_p; cur++)
      if (IS_CONS(*cur)) {
        *UPTR_PTR(*cur) |= GC_FLAG;
        for(cadr = cur+1; IS_CADR(*cadr); cadr++) 
          *cadr |= GC_FLAG;
        *cadr |= GC_FLAG;
      }

    int gcount = 0, non_gcount = 0;
    for (cur = CSTART_p; cur < CEND_p; cur++)
      IS_GARBAGE(*cur) ? gcount++ : non_gcount++;

    printf_P(PSTR("Total words: %i\t%SGarbage words: %i\t Non-garbage words: %i\n"), CONSMEM() / sizeof(uptr_t), CONSMEM() < 200 ? PSTR("\t") : PSTR(""), gcount, non_gcount);

    garbage = 0;
    cur = CSTART_p;
    int gb_total_bytes = 0;
    while (cur < CEND_p) {
      if (IS_GARBAGE(*cur)) {
        garbage = 1;
        uptr_t *lend = cur+1;
        while (IS_GARBAGE(*lend) && lend < CEND_p) lend++;

        int gb_size = lend - cur;
        CSTART_p = (uptr_t *)memmove( CSTART_p + gb_size,
                                      CSTART_p,
                                      (intptr_t)cur - (intptr_t)CSTART_p );
        gb_total_bytes += gb_size * sizeof(uptr_t);
        cur = lend;
      } else
        cur++;
    }

    for (cur = iter_CSTART_p; cur < CEND_p; ++cur)
      if (IS_CONS(*cur) && TO_PTR(*cur) < UPTR(CSTART_p))
        *cur += gb_total_bytes;

    if (ENV_p < UPTR(CSTART_p)) ENV_p += gb_total_bytes;

    for (cur = CSTART_p; cur < CEND_p; ++cur) *cur &= ~GC_FLAG;
  } while (garbage);

  memset((void*)SEND_p, 0, FREEMEM());
  printf_P(PSTR("** GC COMPLETE **\n\n"));
  /*
  printf_P(PSTR("Cons mem: { "));
  for (cur = CSTART_p; cur < CEND_p; cur++) { printf_P(PSTR("%p: "), cur); print_form(*cur); printf_P(PSTR(", ")); }
  printf_P(PSTR("}\n"));
  */
  printf_P(PSTR("env: "));
  print_form(ENV_p);
  printf_P(PSTR("\n"));
  printf_P(PSTR("Total mem:\t%dB\nFree mem:\t%dB\tUsed mem:\t%dB\nCons mem:\t%dB\tSymbol mem:\t%dB\n"),
           TOTALMEM(), FREEMEM(), USEDMEM(), CONSMEM(), SYMMEM());

  return (intptr_t)CSTART_p - (intptr_t)orig_CSTART_p;
}
