#ifndef _ALIST_H
#define _ALIST_H

/* Returns value at k or NIL if not found. */
uptr_t get(uptr_t alist, uptr_t k);

/* Associates k with v, manipulating alist in place. */
void assoc(uptr_t *alist, uptr_t k, uptr_t v);

#endif
