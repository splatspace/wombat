#include <string.h>
#include <ctype.h>

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
    *UPTR_PTR(CSTART_p) = car;
    *UPTR_PTR(CSTART_p + sizeof(uptr_t)) |= CADR_FLAG;
    return CSTART_p;
  } 

  CSTART_p -= sizeof(Cons);
  Cons *new_cons = (Cons*)CPTR(CSTART_p);
  new_cons->car = car;
  new_cons->cdr = cdr;
  return UPTR(new_cons);
}

uptr_t symbol_alloc(char *name) {
  int len = strlen(name);
  if (len > 6) len = 6;

  if (len <= 4) {
    memcpy(CPTR(SEND_p), name, len);
    *SYM_PTR(SEND_p) |= LIT_SYM_FLAG;
  } else {
    int i;
    for (i = 0; i < len; ++i) {
      uint32_t *new_sym = SYM_PTR(SEND_p);
      char cur = name[i];

      if (isalpha(cur))
        *new_sym |= (uint32_t)((char)toupper(cur) - 'A' + 1);
      else
        *new_sym |= 27;

      *new_sym <<= 5;
    }

    if (len == 5) *new_sym <<= 5;
  }

  SEND_p += 4;
  return SEND_p - 4;
}
