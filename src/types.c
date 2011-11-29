#include <uberlisp/types.h>

void init_mem() {

  CSTART_p = CEND_p = (uptr_t *)(&__heap_start);
  SSTART_p = SEND_p = (uint32_t *)(&__bss_end);

  memset(SSTART_p, 0, TOTALMEM());
}

uptr_t build_cons(uptr_t car, uptr_t cdr) {
  if (FREEMEM() < sizeof(uptr_t)) __GC__();

  if (IS_PTR(cdr) && cdr == UPTR(CSTART_p)) {
    CSTART_p--;
    *CSTART_p = car;
    *(CSTART_p + 1) |= CADR_FLAG;
    return UPTR(CSTART_p);
  } 

  CSTART_p -= 2;
  Cons *new_cons = (Cons*)CSTART_p;
  new_cons->car = car;
  new_cons->cdr = cdr;
  return UPTR(new_cons);
}

inline void __mk_sym(uint32_t s) {
  *SEND_p = s;
  SEND_p++;
}

uptr_t build_symbol(char *name) {
  *SEND_p = hash_sym(name);

  uint32_t *finder = SSTART_p;

  while (*finder != *SEND_p) ++finder;

  if (finder == SEND_p)
    SEND_p++;
  else
    *SEND_p = 0;
  
  return UPTR(finder);
}

inline void __set_env(uptr_t *env) {
  ENV_p = env;
}

uint32_t hash_sym(char *name) {
  int len = strlen(name);
  if (len > 6) len = 6;

  uint32_t hash = 0;

  if (len <= 4) {
    int i;
    for (i = 0; i < len; i++)
      ((char*)&hash)[i] = (char)toupper(name[i]);
    
    hash |= LIT_SYM_FLAG;
  } else {
    int i;
    for (i = 0; i < len; ++i) {
      char cur = name[i];

      if (isalpha(cur))
        hash |= ((uint32_t)((char)toupper(cur) - 'A' + 1)) << (5*i);
      else
        hash |= ((uint32_t)USCORE_HSH) << (5*i);
    }
  }

  return hash;
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
