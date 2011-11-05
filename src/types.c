#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <uberlisp/uberlisp.h>
#include <uberlisp/types.h>

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

uptr_t build_symbol(char *name) {
  hash_sym(SEND_p, name);

  uptr_t finder = SSTART_p;

  while (*SYM_PTR(finder) != *SYM_PTR(SEND_p)) finder += 4;

  if (finder == SEND_p)
    SEND_p += 4;
  else
    memset(CPTR(SEND_p), 0, 4);
  
  return finder;
}

void hash_sym(uptr_t store, char *name) {
  int len = strlen(name);
  if (len > 6) len = 6;

  uint32_t *hash = SYM_PTR(store);

  if (len <= 4) {
    memcpy(hash, name, len);
    *hash |= LIT_SYM_FLAG;
  } else {
    int i;
    for (i = 0; i < len; ++i) {
      char cur = name[i];

      if (isalpha(cur))
        *hash |= ((uint32_t)((char)toupper(cur) - 'A' + 1)) << (5*i);
      else
        *hash |= USCORE_HSH;
    }
  }
}

void unhash_sym(char *buf, uptr_t sym_p) {
  uint32_t hash = *SYM_PTR(sym_p);

  if (hash & LIT_SYM_FLAG) {
    hash &= ~LIT_SYM_FLAG;
    strncpy(buf, (char *)&hash, 4);
  } else {
    char *cur = buf;
    while(hash) {
      *cur = (char)(hash & ((uint32_t)0x1F));
      if (*cur == USCORE_HSH)
        *cur = '_';
      else
        *cur = *cur - 1 + 'A';

      hash >>= 5;
      ++cur;
    }
  }
}
