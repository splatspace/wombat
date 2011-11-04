#include <stdlib.h>
#include <string.h>
#include "types.h"

void hash_sym(uptr_t store, char *name) {
  int len = strlen(name);
  if (len > 6) len = 6;

  uint32_t &hash = SYM_PTR(store);

  if (len <= 4) {
    memcpy(&hash, name, len);
    hash |= LIT_SYM_FLAG;
  } else {
    int i;
    for (i = 0; i < len; ++i) {
      char cur = name[i];

      if (isalpha(cur))
        hash |= (uint32_t)((char)toupper(cur) - 'A' + 1);
      else
        hash |= USCORE_HSH;

      hash <<= 5;
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
        *cur = unhashed_c - 1 + 'A';

      hash >>= 5;
      ++cur;
    }
  }
}
