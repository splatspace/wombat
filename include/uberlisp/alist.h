#ifndef _UBERLISP_ALIST_H
#define _UBERLISP_ALIST_H

#include <uberlisp/types.h>

/* Returns value at k or NIL if not found. */
uptr_t get(uptr_t k);

/* Associates k with v, manipulating alist in place. */
void assoc(uptr_t k, uptr_t v);

void __set_binding(uptr_t k, uptr_t v);

#endif
