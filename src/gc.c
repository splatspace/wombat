#include <uberlisp/gc.h>

void __GC__() {
  *ENV_p |= GC_FLAG;
  uptr_t *cur;
  int orphans;

  do {
    for (cur = CSTART_p; cur < CEND_p; cur++)
      if (IS_CONS(*cur)) {
        *UPTR_PTR(*cur) |= GC_FLAG;
        if (!IS_CADR(__CDR(*cur))) *UPTR_PTR(__CDR(*cur)) |= GC_FLAG;
      }
  
    orphans = 0;
    for (cur = CSTART_p; cur < CEND_p; cur++)
      if (IS_ORPHAN(*cur)) orphans++;

    if (orphans) {
      cur = CSTART_p;
      while (cur < CEND_p) {
        if (IS_ORPHAN(*cur)) {
          uptr_t *lend = cur;
          while (*lend && !IS_CONS(*lend)) lend++;

          lend++;

          int osize = (intptr_t)lend - (intptr_t)cur;
          int csize = (intptr_t)cur - (intptr_t)CSTART_p;
          memmove((void*)CSTART_p + osize, (void*)CSTART_p, csize);

          uptr_t *cmpct;
          for (cmpct = lend; cmpct < CEND_p; ++cmpct)
            if (IS_CONS(*cmpct) && TO_PTR(*cmpct) < UPTR(cur))
              *cmpct += osize;

          cur = lend;
          CSTART_p += csize;
        } else
          cur++;
      }
    }
    for (cur = CSTART_p; cur < CEND_p; ++cur) *cur &= ~GC_FLAG;
  } while (orphans);

  memset((void*)SEND_p, 0, FREEMEM());
}
