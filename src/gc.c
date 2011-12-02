#include <uberlisp/gc.h>
#include <uberlisp/print_form.h>

void __GC__() {
  printf_P(PSTR("\n** BEGIN GC **\n"));
  uptr_t *cur;
  int orphans;

  do {
    *UPTR_PTR(ENV_p) |= GC_FLAG;
    printf_P(PSTR("Marking..."));
    for (cur = CSTART_p; cur < CEND_p; cur++)
      if (IS_CONS(*cur)) {
        *UPTR_PTR(*cur) |= GC_FLAG;
        if (!IS_CADR(__CDR(*cur))) *UPTR_PTR(__CDR(*cur)) |= GC_FLAG;
      }
    printf_P(PSTR("done.\n"));

    orphans = 0;
    printf_P(PSTR("Counting orphans..."));
    for (cur = CSTART_p; cur < CEND_p; cur++)
      if (IS_ORPHAN(*cur)) orphans++;
    printf_P(PSTR("done. %i orphans\n"), orphans);

    if (orphans) {
      cur = CSTART_p;
      printf_P(PSTR("Begin compacting...\n"));
      while (cur < CEND_p) {
        if (IS_ORPHAN(*cur)) {
          printf_P(PSTR("Found orphan: %p\toffset: %i\n"), cur, (intptr_t)cur - (intptr_t)CSTART_p);
          uptr_t *lend = cur;
          while (*lend && !IS_CONS(*lend) && lend < CEND_p-1) lend++;

          lend++;

          int osize = lend - cur;
          int csize = cur - CSTART_p;
          printf_P(PSTR("Orphaned list ends at: %p\tlength: %i\tend offset: %i\t\n"), lend, osize, (intptr_t)CEND_p - (intptr_t)lend);
          printf_P(PSTR("memmove(CSTART_p + osize, CSTART_p, csize * sizeof(uptr_t)) => memmove(%p, %p, %i)\n"), CSTART_p + osize, CSTART_p, csize * sizeof(uptr_t));
          memmove(CSTART_p + osize, CSTART_p, csize * sizeof(uptr_t));

          uptr_t *cmpct;
          printf_P(PSTR("Adjusting pointers...\n"));
          for (cmpct = lend; cmpct < CEND_p; ++cmpct)
            if (IS_CONS(*cmpct) && TO_PTR(*cmpct) < UPTR(cur)) {
              printf_P(PSTR("Found pointer!\n\tcur val: %x\tcur unflagged val: %x\n\tnew val: %x\tnew flagged val:%x\n"), 
                       *cmpct, TO_PTR(*cmpct), *cmpct + osize, TO_PTR(*cmpct + osize));
              *cmpct += osize;
            }
          printf_P(PSTR("Done adjusting.\n"));

          if (ENV_p < UPTR(cur)) ENV_p += osize * sizeof(uptr_t);
          cur = lend;
          CSTART_p += osize;
        } else
          cur++;
      }
    }
    printf_P(PSTR("Clearing GC flags..."));
    for (cur = CSTART_p; cur < CEND_p; ++cur) *cur &= ~GC_FLAG;
    printf_P(PSTR("done.\n"));
  } while (orphans);
  printf_P(PSTR("Done compacting.\n"));

  printf_P(PSTR("Zeroing FREEMEM..."));
  memset((void*)SEND_p, 0, FREEMEM());
  printf_P(PSTR("done.\n"));
  printf_P(PSTR("** GC COMPLETE **\n\n"));
  printf_P(PSTR("Cons mem: { "));
  for (cur = CSTART_p; cur < CEND_p; cur++) { printf_P(PSTR("%p: "), cur); print_form(*cur); printf_P(PSTR(", ")); }
  printf_P(PSTR("}\n"));
  printf_P(PSTR("env: "));
  print_form(ENV_p);
  printf_P(PSTR("\n"));
}
