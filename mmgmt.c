#include "mmgmt.h"
#include "types.h"

void init_mem() {
  CSTART_p = CEND_p = 0x880; /* Leave 128B for stack */

  /* TODO: This isn't Arduino compatible */
  SSTART_p = SEND_p = UPTR(HEAP_p);
}

uptr_t build_cons(uptr_t car, uptr_t cdr) {
  if (IS_PTR(cdr) && cdr == CSTART_p) {
    CSTART_p -= sizeof(uptr_t);
    DRF_UPTR(CSTART_p) = car;
    DRF_UPTR(CSTART_p + sizeof(uptr_t)) &= CADR_FLAG;
    return CSTART_p;
  } 

  CSTART_p -= sizeof(Cons);
  Cons *new_cons = (Cons*)CPTR(CSTART_p);
  new_cons->car = car;
  new_cons->cdr = cdr;
  return UPTR(new_cons);
}
